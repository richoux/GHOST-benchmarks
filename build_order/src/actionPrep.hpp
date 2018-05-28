#pragma once

struct ActionPrep
{
  ActionPrep( ActionData action, int wait_time, int id )
    : action(action), wait_time(wait_time), id(id) { }
  
  ActionData action;
  int wait_time;
  int id;
};
