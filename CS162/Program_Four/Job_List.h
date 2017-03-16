/*****************************************************************
// File Name:  Job_List.h
// Created By: Nicholas Toth
// E-Mail:     ntoth@pdx.edu
// Date:       3/10/2017
//
// Overview: This is the header file for the Job_List class. 
// Essentially, one instance of this class contains some number
// of jobs, interviews, etc. The operations included are related
// to managing (loading,editing,creating,writing) job a user's job
// information.
//
// ***************************************************************/

#ifndef JOB_LIST_H
#define JOB_LIST_H

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

            // The maximum size of a given line in the exernal files.
const short LINE_BUFFER_SIZE = 255,
            // The maximum size of an entry in a given job.
            MAX_ENTRY_SIZE   = 255,
            // The maximum number of jobs that can be read in from a file.
            MAX_LOAD_SIZE    = 127;



/*******************************************************
// Contains the data for a given job.
//
// *****************************************************/
struct Job
{
    // Active when containing info.
    bool isactive;
    // Name of the company for a given job.
    char company_name[MAX_ENTRY_SIZE + 1];
    // The name of the position for a given job.
    char job_title[MAX_ENTRY_SIZE + 1];
    // The number of hours worked per day for a given job.
    char hours_per_day[MAX_ENTRY_SIZE + 1];
    // The begining date for a given job.
    char start_date[MAX_ENTRY_SIZE + 1];
    // The ending date for a given job.
    char end_date[MAX_ENTRY_SIZE + 1];
    // Personal thoughts about a given job.
    char personal_thoughts[MAX_ENTRY_SIZE + 1];
};



/*******************************************************
// Contains the interface for implementing the Job_List 
// object.
//
// *****************************************************/
class Job_List
{
    public:
     // Job constructor w/o auto open file.
     Job_List(void);
     // Job constructor with auto open file.
     Job_List(const char filename[]);
     // Job copy constructor.
     Job_List(const Job_List &j_list);
     // Job destructor
     ~Job_List(void);
     // Create new job
     void Read(void);
     // Display existing job.
     void Display(void);
     // Edit existing job. 
     void Edit(void);
     // Load/create a file.
     bool Load(const char filename[]);
     // Write job object contents to file.
     void Save(const char filename[]);

    private:
	// Array of job objects
        Job *jobs;
	// Total number of jobs in the jobs array.
        unsigned short counter;
	// Memory size of job object in bytes.
        unsigned int bytes;
	// Prompt for/read in user's selection for member to edit.
        short jobSelectMenu(void);
	// Edit a field on one of the jobs in the jobs array.
        void editField(short edit_job, short edit_field);
};

#endif // JOB_LIST_H
