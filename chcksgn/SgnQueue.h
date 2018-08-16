#pragma once

// шаблон класса Очередь, ограниченной длины

template <class QType>
class SgnQueue
{
	// мьютекс для всех манипуляций с очередью
	mutable std::mutex LockQ;    
	// этот сигнал - очередь не пустая
	std::condition_variable CondP;  
	// этот сигнал - в очереди есть место
	std::condition_variable CondC;       
	//очередь данных
	std::queue<QType> QueueItems;
	// максимальный размер очереди 
	int max;              

public:

	void InitQ(int max)
	{
		this->max = 1;
		if (max > 1) this->max = max;
	}

	void push(QType new_value) {
		std::unique_lock<std::mutex> lk(LockQ);
		CondC.wait(lk, [this] {return(QueueItems.size() < max); });
		QueueItems.push(new_value);
		CondP.notify_all();
	}

	void pop(QType& value) {
		std::unique_lock<std::mutex> lk(LockQ);
		CondP.wait(lk, [this] {return(!QueueItems.empty()); });
		value = QueueItems.front();
		QueueItems.pop();
		CondC.notify_all();
	}
	bool empty()const {
		std::unique_lock<std::mutex> lk(LockQ);
		return QueueItems.empty();
	}
};

