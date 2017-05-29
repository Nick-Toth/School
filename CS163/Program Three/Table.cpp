/*****************************************************************
// File Name:  Table.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       May 24nd, 2017
// Class:      CS163
// Program #:  3
//
// Overview: This is the implementation file for the hash table
// for programming assignment #3. Essentially, this hash table is
// used to store pointers to Events (allocated in a linked list),
// using event keywords. See function comments for more (specific)
// information.
//
// ***************************************************************/

#include "Table.h"


// ***************** |
// Node Functions *  |
// ***************** V


/* ********************************************
// Destructor deallocates eventdata, sets
// pointers to NULL.
//
// ********************************************/
Node :: Node(void)
{
  data = NULL;
  next = NULL;
}


/* ********************************************
// Default constructor sets pointers to NULL.
//
// ********************************************/
ENode :: ~ENode(void)
{
  next = NULL;
}


/* ********************************************
// Default constructor sets pointers to NULL.
//
// ********************************************/
Node :: ~Node(void)
{
  data = NULL;
  next = NULL;
}


// ***************** |
// Table Functions * |
// ***************** V


/* ********************************************
// Table constructor initializes the size of
// the hash table, Initializes the hash table,
// sets all pointers to NULL, fields to 0.
//
// ********************************************/
Table :: Table(unsigned int size)
{
  // Set current events to 0.
  current_size = 0;

  // Index counter for nullifying the table.
  unsigned int index = 0;

  // If a size was passed in..
  if(size != DEFAULT_TABLE_SIZE)
  {
    // If the table size is not at least 4, return!
    if(size < 4) return;
    // Make sure size is an odd number.
    if(size % 2 == 0) ++size;
    // Make sure the size is a prime number.
    size = nextPrime(size);
  }

  // Set the table size.
  table_size = size;
  // Set current events to 0.
  current_size = 0;

  // Set hash_prime to the quotient of the table size and 2.
  hash_prime = size / 2;
  // If hash_prime is an even number, make it an odd number.
  if(hash_prime % 2 == 0) ++hash_prime;
  // Set hash_prime to its closest (+) prime.
  hash_prime = nextPrime(hash_prime);

  // Set the event list to NULL.
  events = NULL;

  // Allocate memory for the hash table.
  hash_table = new Node * [size];

  // Nullify the hash table.
  for(; index < size; ++index)
    hash_table[index] = NULL;

  return;
}



/* ********************************************
// Copy constructor for future use.
//
// ********************************************/
Table :: Table(const Table & table)
{ return; }



/* ********************************************
// Deallocate all events in the hash table.
//
// ********************************************/
Table :: ~Table(void)
{
      				 // Index counter.
  unsigned int index = 0,
      				 // Deleted node counter.
      				 deleted = 0;

  // Temporary node for traversing events.
  ENode * temp;

  // If the event list isn't NULL..
  if(events)
  {
    // Get the first event.
    temp = events;

    // While events isn't NULL..
    while(events)
    {
      // Store the next event.
      temp = events->next;

      // Delete the head of the event list.
      delete events;

      // Set events to the stored next node.
      events = temp;

      // Increment the deleted event counter.
      ++deleted;
    }
  }

  // For every Node in the hash table.
  for(; index < table_size; ++index)
  {
    // If the current Node isn't NULL, delete it.
    if(hash_table[index]) delete hash_table[index];
    // Set the current Node to NULL.
    hash_table[index] = NULL;
  }

  // Subtract the number of deleted nodes from current_size
  current_size -= deleted;

  // Deallocate and nullify the hash table.
  delete[] hash_table;
  hash_table = NULL;
}



/* ********************************************
// Inserts a new event into the ENode list, and
// a new node into the hash table for each
// keyword in the new event. Returns the number
// of successfully hashed keywords.
//
// ********************************************/
int Table :: insert(const Event & new_event)
{
  // Get the number of keywords in the new event.
  int keywords = new_event.keywordCount(),
      // Index for hashing the event.
      h_index = 0,
      // Index counter for traversing keywords.
      k_index = 0;

  // Loop control for checking if a node
  // already exists in a given chain
  bool is_first = true;

         // Like date_node, the keyword nodes' points
         // to the event allocated in new_node.
  Node * keyword_nodes[5],
         // Temp node for checking if a node
         // already exists in a given chain.
       * temp = NULL;

  // Create a new enode for storing the event.
  ENode * new_enode = new ENode;

  // Copy the new event into the new enode.
  new_enode->data.copy(new_event);

  // Set the new enode to point to the other enodes.
  new_enode->next = events;
  events = new_enode;

  // Insert pointers using keywords.
  for(; k_index < keywords; ++k_index)
  {
    // Reset is_first.
    is_first = true;

    // Get the index for the current keyword.
    h_index = hash(new_event.keywords[k_index]);

    // Get the node of the hash table at h_index.
    temp = *(hash_table + h_index);

    // Check if the node already exists in the chain.
    while(temp && temp->data && strcmp(temp->data->name, new_event.name))
    {
       // Set is_first to the result of comparing
       // the current node in the chain to the new event.
      is_first = strcmp(temp->data->name, new_event.name);

      // If the names didn't match, set temp to the next node.
      if(is_first) temp = temp->next;
    }

    // If the node is not already in the chain..
    if(is_first)
    {
      keyword_nodes[k_index] = new Node;

      // Point the new hash table node's data to new_enode's data.
      keyword_nodes[k_index]->data = &new_enode->data;

      // Set the new node's next pointer to the
      // current value at the hashed index.
      keyword_nodes[k_index]->next = *(hash_table + h_index);

      // Set the hash table at the
      // hashed index to the new node.
      *(hash_table + h_index) = keyword_nodes[k_index];
    }
  }

  // Increment the event counter.
  ++current_size;

  // Return the number of hashed keywords.
  return k_index;
}



/* ********************************************
// Retrieves up to 'found' events which contain
// the specified keyword from the table. Stores
// results in the event array. Returns the
// number of results.
//
// ********************************************/
int Table :: retrieve( const char * keyword,
                       Event event_match[],
                       int & found ) const
{
  // If the keyword is NULL, return
  // error code -1 immediately.
  if(!keyword) return -1;

      // Index of the hash table given
      // by the specified keyword.
  int h_index = hash(keyword),
      // General index counter.
      g_index = 0,
      // The number of keywords in a
      // given node's event.
      keyword_count = 0,
      // Current number of matches.
      current_total = 0;

  // True when a match is found.
  bool match = false;

  // Pointer to the hed of the chain at the index
  // of the hash table given by the keyword param.
  Node * temp = hash_table[h_index];

  // While the current node is not NULL, and the number
  // of matches does not exceed the max, given by found..
  while(temp && current_total < found)
  {
    // Get the number of keywords in the current node's event.
    keyword_count = temp->data->keywordCount();
    // Reset match to false.
    match = false;
    // Reset general index counter to 0.
    g_index = 0;

    // While there are unchecked keywords,
    // and no match has been found yet..
    while(g_index < keyword_count && !match)
    {
      // If the keyword param matches the current keyword
      // in the current node's event, set match to true.
      if(!strcmp(temp->data->keywords[g_index], keyword)) match = true;

      // Increment the index counter.
      ++g_index;
    }

    // If a match was found..
    if(match)
    {
      // Copy the contents of the matched event.
      event_match[current_total].copy(*temp->data);
      // Increment the total match counter.
      ++current_total;
    }

    // Set temp to the next node in the chain.
    temp = temp->next;
  }

  // Set found to, and return the number of matches.
  return found = current_total;
}



/* ********************************************
// Computes an integer to be used as an index
// in the hash table based on a key. Returns
// the result.
//
// ********************************************/
unsigned int Table :: hash(const char * key) const
{

  // If the key is NULL, return -1.
  if(!key) return -1;

      				 // Index to be returned.
  unsigned int hash_val = 1,
							 // Length of the key.
							 len = strlen(key),
      				 // Index counter for key.
      				 index = 0;


  // For each character in the key..
  for(; index < len; ++index)
    // Add hash_prime to the key, and multiply that number
    // by the ascii value of the current index of the key.
    hash_val = hash_prime + hash_val + *(key + index);

  // Calculate & return the index.
  return hash_val % table_size;
}



/* ********************************************
// If num is prime, returns num. Otherwise,
// returns the next prime number after num.
// Used whenever the constructor is given a
// specific table size to ensure that the table
// size is prime. Warning, this function assumes
// that the number being passed in is an odd
// number greater than 3. If num = 3, 5 will be
// returned. If a multiple of two is passed in,
// it will be returned. The reason for this is
// that nextPrime is recursive, so there would
// be redundant checks. This function is called
// only by the constructur, which ensures that
// num meets these criteria. It should not be
// used anywhere else - unless the "CHECK 2"
// line is uncommented.
//
// ********************************************/
int Table :: nextPrime(unsigned int num) const
{
               // Divisor counter. 5 += 2 .. < sqrt num.
  unsigned int d = 3,
               // Max possible divisor.
               max = static_cast<unsigned int>(sqrt(num)) + 1;

  // Ensure that num is an odd number.
  // if(num % 2 == 0) ++num; // CHECK 2.

  // While d does not divide num, and d is less
  // than sqrt num, add two to the divisor.
  while(d < max && num % d != 0) d += 2;

  // If the last divisor divides num, try
  // again with the next odd number.
  if(num % d == 0) return nextPrime(num + 2);

  // The number is prime.
  return num;
}



/* ********************************************
// Displays all of the events which contain
// a specific keyword. Return the number of
// displayed events (matched).
//
// ********************************************/
int Table :: display(const char * keyword) const
{
      // Index of the hash table given
      // by the specified keyword.
  int h_index = hash(keyword),
      // General index counter.
      g_index = 0,
      // Number of keywords in an event.
      keyword_count = 0,
      // The number of displayed events.
      displayed = 0;

  // True if a keyword match is found.
  bool match = false;

  // Display alert that events are being printed.
  std::cout << "\n  Displaying events with the keyword: " << keyword << std::endl;

  // Get the head of the chain given
  // by hashing the keyword.
  Node * temp = hash_table[h_index];

  // Until temp is NULL..
  while(temp)
  {
    // Get the number of keywords in the current event.
    keyword_count = temp->data->keywordCount();

    // Reset locals
    match = false;
    g_index = 0;

    // While there are unchecked keywords
    //  and a match hasn't yet been found..
    while(g_index < keyword_count && !match)
    {
      // If the current keyword matches, the
      // keyword param, set match to true.
      if(!strcmp(temp->data->keywords[g_index], keyword)) match = true;
      // Increment the index counter.
      ++g_index;
    }
    // If a match was found..
    if(match)
    {
      // Display the current event.
      temp->data->display();
      // Increment the displayed node counter.
      ++displayed;
    }
    // Set temp to the next node in the chain.
    temp = temp->next;
  }

  // Return the number of displayed events.
  return displayed;
}



/* ********************************************
// Displays the event in every ENode. Returns
// the number of displayed events.
//
// ********************************************/
int Table :: display(void) const
{

  // Number of unique events displayed.
  short total_displayed = 0;

  // Create a pointer to the events.
  ENode * temp = events;

  // Display alert that events are being printed.
  std::cout << "\n  Displaying Events.." << std::endl << std::endl;

  // For every unique event..
  while(temp)
  {
    // Display the current event.
    temp->data.display();
    // Set temp to the next event node.
    temp = temp->next;
    // Increment the displayed event counter.
    ++total_displayed;
  }

  // Return the number of displayed events.
  return total_displayed;
}



/* ********************************************
// Deallocates every event that is older than
// the specified date. Returns the number of
// deleted nodes.
//
// ********************************************/
int Table :: removeOld(const char * date)
{
  // Number of deleted events.
  int deleted = 0;

  // True when an event from before date is found.
  bool older = false;

          // Create a pointer to the events.
  ENode * current = events,
          // Create a temp pointer for reattatching
          // the event list.
        * temp = NULL,
          // Save next pointer before deleting.
        * save = NULL;

  // For every unique event..
  while(current)
  {
    // Check if the current event is
    // older than the specified date.
    older = datecmp(current->data.date, date);

    // If the current event is not current (get it?)..
    if(older)
    {
      // If temp is not NULL (it is the node before current)..
      if(temp)
        // Set temp's next to current's next.
        temp->next = current->next;
      // Otherwise (it must be the front of the list).
      else
        // Set events head to the next node.
        events = current->next; // == events = events->next; 

      // Save the node after current.
      save = current->next;

      // Delete the old node.
      delete current;
      // Set current to the next node.
      current = save;

      // Increment the deleted event counter.
      ++deleted;
    }
    else
    {
      // Set temp to current (so that the
      // list can be reattatched later)
      temp = current;
      // Set current to the next node.
      current = current->next; 
    }
  }

  // Return the number of deleted nodes.
  return deleted;
}



/**********************************************************
// Compares two time stamps. Returns true if fst_date
// is older than snd_date.
//
// ********************************************************/
bool Table :: datecmp( const char * fst_date,
                       const char * snd_date ) const
{
          // Short integer cast values in the time stamp arrays.
    short current_digit_1 = 0,
          current_digit_2 = 0,
          // The offset from the current index of the ords
          // array used to compare indices in the time stamps.
          offset = 0,
          // The current index of the ords array to be used in
          // comparing the indices of the time_stamps.
          index  = 0,
          // An array of section separaters for time stamps.
          // [0] => year, [1] => month, [2] => day.
          ords[3] = {6,3,0};

         // Loop control variable for comparing time stamps.
    bool age_confirmed = false,
         // Set to true when age is confirmed.
         is_older = false;

    // Until the relative age of the timestamps has been confirmed..
    while(!age_confirmed)
    {
        // Case the current index (by offset from current ord,
        // not by iteration) of each timestamp to short int, store.
        current_digit_1 = static_cast<short>( fst_date[ ords[ index ] + offset ] - '0' );
        current_digit_2 = static_cast<short>( snd_date[ ords[ index ] + offset ] - '0' );

        // If the current numbers of the indices of the time stamps
        // are not the same..
        if( current_digit_1 > current_digit_2 || current_digit_1 < current_digit_2  )
        {
            // Set confirmed to true so that the loop can discontinue.
            age_confirmed = true;
            // If the first is not older than the second, set isolder
            // to false. Otherwise, set to true.
            is_older = !(current_digit_1 > current_digit_2);
        }
        // Otherwise..
        else
            // Increment the offset to check the next offset/section
            // of the timestamps.
            ++offset;

        // If the current section of the timestamps has been fully
        // checked, reset the offset and increment the ords index
        // counter.
        if( (offset == 2 && index == 1) || (offset == 4 && index == 0) )
        {
            // Reset the index offset.
            offset = 0;
            // Increment the ords index counter.
            ++index;
        }
        // Otherwise, if the timestamps are the same..
        else if(offset == 2 && index == 2)
        {
            // Set confirmed to true so that the loop can discontinue.
            age_confirmed = true;
            // The first timestamp is not older than the second.
            is_older = false;
        }
    }
    /*
    std::cout << std::boolalpha << "\n\tRESULT: "
              << fst_date << " older than "
              << snd_date << "? => " << is_older << std::endl << std::endl;
    */
    // Return the result of the compared timestamps.
    return is_older;
}
