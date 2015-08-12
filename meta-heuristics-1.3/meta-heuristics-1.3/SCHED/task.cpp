#ifndef TASK_SOLUTION_H
#define	TASK_SOLUTION_H

#include <fstream>
 class task{
public:	int task_id;
	int task_lenght;
	int task_deadline;

	int getTaskDeadline() const
	{
		return task_deadline;
	}

	void setTaskDeadline(int taskDeadline) {
		task_deadline = taskDeadline;
	}

	int getTaskId() const {
		return task_id;
	}

	void setTaskId(int taskId) {
		task_id = taskId;
	}

	int getTaskLenght() const {
		return task_lenght;
	}

	void setTaskLenght(int taskLenght) {
		task_lenght = taskLenght;
	}
}
;
#endif
