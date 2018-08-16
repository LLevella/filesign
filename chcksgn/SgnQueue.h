#pragma once

// ������ ������ �������, ������������ �����

template <class QType>
class SgnQueue
{
	// ������� ��� ���� ����������� � ��������
	mutable std::mutex LockQ;    
	// ���� ������ - ������� �� ������
	std::condition_variable CondP;  
	// ���� ������ - � ������� ���� �����
	std::condition_variable CondC;       
	//������� ������
	std::queue<QType> QueueItems;
	// ������������ ������ ������� 
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

