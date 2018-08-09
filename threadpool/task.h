#pragma once
class Task
{
public:
	Task();
	virtual ~Task();

	virtual void run() = 0;

	void setPriority(int);
	int getPriority();
	int getId();

private:
	int m_Id, m_Priority;
	static int m_CurrentId;
};

