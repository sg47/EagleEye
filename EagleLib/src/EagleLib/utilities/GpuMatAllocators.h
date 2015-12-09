#pragma once
#include <EagleLib/Defs.hpp>
#include <opencv2/core/cuda.hpp>
#include <mutex>
#include <tuple>
#include <list>
#include <map>
#include <memory>


namespace EagleLib
{
	cv::cuda::GpuMat::Allocator* GetDefaultBlockMemoryAllocator();
	cv::cuda::GpuMat::Allocator* GetDefaultDelayedDeallocator();
	cv::cuda::GpuMat::Allocator* CreateBlockMemoryAllocator();
	
	class EAGLE_EXPORTS PitchedAllocator : public cv::cuda::GpuMat::Allocator
	{
	public:
		PitchedAllocator();
		void SizeNeeded(int rows, int cols, int elemSize, size_t& sizeNeeded, size_t& stride);
		void Increment(unsigned char* ptr, size_t size);
		void Decrement(unsigned char* ptr, size_t size);
		void SetScope(const std::string& name);
	protected:
		size_t textureAlignment;
		size_t memoryUsage;
		std::recursive_mutex mtx;
		std::map<std::string, size_t> scopedAllocationSize;
		std::string currentScopeName;
		std::map<unsigned char*, std::string> scopeOwnership;
	};

	class MemoryBlock;

	class EAGLE_EXPORTS BlockMemoryAllocator: public PitchedAllocator
	{
	
	public:
		static BlockMemoryAllocator* Instance(size_t initial_size = 10000000);
		BlockMemoryAllocator(size_t initialBlockSize);
		virtual bool allocate(cv::cuda::GpuMat* mat, int rows, int cols, size_t elemSize);
		virtual void free(cv::cuda::GpuMat* mat);
		size_t initialBlockSize_;
	protected:
		std::list<std::shared_ptr<MemoryBlock>> blocks;
	};

	class EAGLE_EXPORTS DelayedDeallocator : public PitchedAllocator
	{
	public:
		DelayedDeallocator();
		virtual bool allocate(cv::cuda::GpuMat* mat, int rows, int cols, size_t elemSize);
		virtual void free(cv::cuda::GpuMat* mat);
		size_t deallocateDelay; // ms
		
	protected:
		virtual void clear();
		std::list<std::tuple<unsigned char*, clock_t, size_t>> deallocateList;
	};

	class EAGLE_EXPORTS CombinedAllocator : public DelayedDeallocator //, private BlockMemoryAllocator
	{
	public:
		/* Initial memory pool of 10MB */
		/* Anything over 1MB is allocated by DelayedDeallocator */
		static CombinedAllocator* Instance(size_t initial_pool_size = 10000000, size_t threshold_level = 1000000);
		CombinedAllocator(size_t initial_pool_size = 10000000 , size_t threshold_level = 1000000);
		virtual bool allocate(cv::cuda::GpuMat* mat, int rows, int cols, size_t elemSize);
		virtual void free(cv::cuda::GpuMat* mat);
	protected:
		size_t _threshold_level;
		size_t initialBlockSize_;
		std::list<std::shared_ptr<MemoryBlock>> blocks;
	};
}