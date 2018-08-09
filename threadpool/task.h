#pragma once
class Task
{
public:
	Task();
	virtual ~Task();

	virtual void run() = 0;

	void setPriority(int);
	int getPriority();

private:
	int m_Priority;
};

class CancelTask : public Task
{
public:
	void run();
};