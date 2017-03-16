/*****************************************************************
// File Name:  TaskList.h
// Created By: Nicholas Toth
// E-Mail:     ntoth@pdx.edu
// Date:       3/16/2017
// Program #:  5
//
// Overview: This is the interface file for the TaskList class,
// as well as the Task struct. The functions prototyped  in this
// file, and implemented in the TaskList.cpp file, are used to
// maintain a list of tasks that must be completed in order to
// accomplish some goal.
// 
// ***************************************************************/

#ifndef TASKLIST_H
#define TASKLIST_H

#include <iostream>
#include <fstream>
#include <cstring>

// The maximum size for a line (from a file and directly from the user).
const short LINE_BUFFER_SIZE = 255;

/*******************************************
// The TaskList class contains a linked list
// of user defined Tasks, as well as a
// set of operations for maintaining the
// list.
// 
// *****************************************/
class TaskList
{
    /*******************************************
    // The Task node contains the user's
    // description the task.
    // 
    // *****************************************/
    struct Task
    {
      // The user's description for the task.
      char *task;
      // Pointer to the next task in the list
      Task *next;

      // Task constructor.
      Task(void);
      // Task copy constructor.
      Task(const Task &task_copy);
      // Task destructor deallocates task data and next task.
      ~Task(void);
    };

  public:

    // Constructor for initializing task list head and task count.
    TaskList(void);
    // Copy constructor.
    TaskList(const TaskList &tl);
    // Destructor for deallocating the task list.
    ~TaskList(void);

    // Adds a task to the end of the list.
    void pushTask(char* task);
    // Remove the last task in the list.
    bool removeTask(void);
    // Add a new task to the end of the list.
    bool newTask(void);
    // Edit the contents of an existing task (not the label).
    bool editTask(short task_selection);
    // Edit the label (head) of the task list.
    bool editLabel(void);
    // Display all of the tasks.
    void displayTasks(void);
    // Display a single task by number.
    void displayTask(void);
    // Get the number of tasks (including the label) in the list.
    short getSize(void);
    // Read in the contents of the external file to the list of tasks.
    // If the file is empty, then initialize its label.
    bool readInTaskFile(const char filename[]);
    // Write the contents of the list of tasks to the external file.
    void writeToTaskFile(const char filename[]);

  private:

    // The first node in the list of tasks. Contains a label.
    Task *head;
    // The number of tasks (including the label) in the list.
    short task_count;
};

#endif //TASKLIST_H
