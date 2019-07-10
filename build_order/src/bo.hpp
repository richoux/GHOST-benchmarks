#pragma once

struct BO
{
  BO() { }
  
  BO( string full_name, int start_time, int completed_time )
    : full_name(full_name), start_time(start_time), completed_time(completed_time) { }
  
  string full_name;
  int start_time;
  int completed_time;
};
