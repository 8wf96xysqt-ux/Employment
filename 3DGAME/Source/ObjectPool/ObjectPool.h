#pragma once
#include <vector>
#include <memory>

template<typename T>
class ObjectPool
{
public:
	ObjectPool(size_t size)
	{
		m_Objects.reserve(size);

		for (size_t i = 0; i < size; ++i)
		{
			m_Objects.push_back(std::make_unique<T>());
		}
	}

	T* Get()
	{
		for (auto& object : m_Objects)
		{
			if (!object->IsActive())
			{
				object->Reset();
				return object.get();
			}
		}

		return nullptr;
	}

	std::vector<std::unique_ptr<T>>& GetObjects()
	{
		return m_Objects;
	}

private:
	std::vector<std::unique_ptr<T>> m_Objects;
};