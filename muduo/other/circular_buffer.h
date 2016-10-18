#ifndef MUDUO_OTHER_CIRCULAR_BUFFER_H
#define MUDUO_OTHER_CIRCULAR_BUFFER_H

namespace muduo
{
	template<class valueType>
	class circular_buffer
	{
	public:
		//覆盖默认构造函数，后续需要调用
		circular_buffer():circularBufferStart(0),
				circularBufferEnd(circularBufferStart),
				circularBufferLen(1),
				valuePtr(NULL)
		{

		}
		//构造函数，后面没有必要调用initialize
		//因为要形成一个前闭后开的空间，所以cycdeque_len_比实际要求的数据长度+1
		circular_buffer(size_t len):circularBufferStart(0),
				circularBufferEnd(circularBufferStart),
				circularBufferLen(len+1),
				valuePtr(NULL)
		{
			assert(len > 0);
        	valuePtr = new valueType[circularBufferLen];
		}
		~circular_buffer()
		{
			if(valuePtr)
			{
				delete[] valuePtr;
				valuePtr = NULL;
			}
		}
		//因为要形成一个前闭后开的空间，所以len比实际要求的数据长度+1
    	void initialize(size_t len)
    	{
    	    assert(len > 0);
 	
    	    circularBufferStart =0;
    	    circularBufferEnd = circularBufferStart;
    	    circularBufferLen = len+1;
    	    //清理现场
    	    if(valuePtr)
    	    {
    	        delete[] valuePtr;
    	        valuePtr = NULL;
    	    }
    	    valuePtr = new valueType[circularBufferLen];
    	}
    	void clear()
    	{
    	    circularBufferStart =0;
    	    circularBufferEnd = circularBufferStart;
    	}
    	//返回circular_buffer的大小
    	inline size_t size() const
    	{
    	    if ( circularBufferEnd >= circularBufferStart )
    	    {
    	        return circularBufferEnd - circularBufferStart;
    	    }
    	    else
    	    {
    	        return circularBufferEnd + circularBufferLen - circularBufferStart;
    	    }
    	}
    	 //返回空闲空间的大小
    	inline size_t freesize() const
    	{
    	    return circularBufferLen - size() - 1;
    	}
    	//返回队列的容量
    	inline size_t capacity() const
    	{
    	    return circularBufferLen - 1;
    	}
    	//检查是否已经满了
    	inline bool full() const
    	{
    	    //如果结束+1%
    	    if((circularBufferEnd + 1) % circularBufferEnd == circularBufferStart)
    	    {
    	        return true;
    	    }
    	    return false;
    	}
 	
    	//判断队列是否为空
    	inline bool empty() const
    	{
    	    //如果发现开始==结束
    	    if(circularBufferStart == circularBufferEnd)
    	    {
    	        return true;
    	    }
    	    return false;
    	}
    	//重新分配一个空间,
    	bool resize(size_t newSize)
    	{
    	    assert(newSize > 0);
 	
    	    size_t dequeSize = size();
    	    //如果原来的尺寸大于新的尺寸，无法扩展
    	    if( dequeSize > newSize )
    	    {
    	        return false;
    	    }
    	    valueType *newValuePtr = new valueType[newSize+1];
 	
    	    //调整几个内部参数
    	    circularBufferStart =0;
    	    circularBufferEnd = dequeSize;
    	    circularBufferLen = newSize+1;
    	   
    	    //如果原来有数据
    	    if(valuePtr != NULL)
    	    {
    	        for (size_t i=0;i<dequeSize;++i)
    	        {
    	            newValuePtr[i] = valuePtr[(circularBufferStart+i)%circularBufferLen];
    	        }
    	        delete[] valuePtr;
    	        valuePtr = NULL;
    	    }
 	
    	    valuePtr = newValuePtr;
    	    return true;
    	}
    	//将一个数据放入队列的尾部,如果队列已经满了,你可以将lay_over参数置位true,覆盖原有的数据
    	bool push_back(const valueType &valueData,bool overWrite =false)
    	{  

    	    if((circularBufferEnd + 1)%circularBufferLen ==circularBufferStart )
    	    {
    	        //如果不要覆盖，返回错误
    	        if(overWrite == false)
    	        {
    	            return false;
    	        }
    	        //如果要覆盖
    	        else
    	        {
    	            //将最后一个位置覆盖，并且调整起始和结束位置
    	            valuePtr[circularBufferEnd] = valueData;
    	            circularBufferStart = (circularBufferStart +1 ) % circularBufferLen;
    	            circularBufferEnd = (circularBufferEnd+1) % circularBufferLen;
    	            return true;
    	        }
    	    }

    	    valuePtr[circularBufferEnd] = valueData;
    	    circularBufferEnd = (circularBufferEnd+1) % circularBufferLen;
    	    return true;
    	}
    	//从队列的前面得到一个数据
    	bool pop_front(valueType &valueData)
    	{
    	    if (size() == 0)
    	    {
    	        return false;
    	    }
    	    valueData = valuePtr[circularBufferStart];
    	    circularBufferStart = (circularBufferStart +1 ) % circularBufferLen;
    	    return true;
    	}
    	bool pop_front()
    	{
    	    if (size() == 0)
    	    {
    	        return false;
    	    }
    	    circularBufferStart = (circularBufferStart +1 ) % circularBufferLen;
    	    return true;
    	}
    	//从队列的后面得到一个数据
    	bool pop_end(valueType &valueData)
    	{
    	    if (size() == 0)
    	    {
    	        return false;
    	    }
 	
    	    circularBufferEnd = (circularBufferEnd > 0)?circularBufferEnd-1:circularBufferEnd-1;
    	    valueData = valuePtr[circularBufferEnd];
    	   
    	    return true;
    	}
    	bool pop_end()
    	{
    	    //
    	    if (size() == 0)
    	    {
    	        return false;
    	    }
    	    circularBufferEnd = (circularBufferEnd > 0)?circularBufferEnd-1:circularBufferEnd-1;
    	    return true;
    	}
    	//ID不要越界，自己保证，我没兴趣为你干什么
    	valueType& operator[](size_t id)
    	{
    		assert(id >= 0 && id < circularBufferLen);
    	    return valuePtr[(circularBufferStart + id) % circularBufferLen];
    	}
	protected:
		//循环队列的起始位置(前开后闭)
		size_t circularBufferStart;
		//循环队列的结束位置(前开后闭)
		size_t circularBufferEnd;
		//队列的长度
		size_t circularBufferLen;
		//存放数据的指针
		valueType *valuePtr;
	};
}
#endif
