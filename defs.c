#include <stdio.h>
#include <stdlib.h>

#define nullptr NULL
#define STEPS_LIMIT_8_BIT 255

enum {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};
enum {NORMAL = 0};

typedef struct Position Position;
typedef struct Position* Position_ptr;

typedef struct Thread Thread;
typedef struct Thread* Thread_ptr;

typedef Steps_type uint64_t;
typedef ID_type uint32_t;
typedef Bock_type uint8_t;

struct Position
{
  uint8_t orientation;
  uint8_t x;
  uint8_t y;
} __attribute__((packed));

struct Thread
{
  Position where;

  Thread_ptr back_ptr;
  uint8_t move_type;

  Thread_ptr step_ptr1, step_ptr2,
  step_ptr3;

  Steps_type number_steps;
};

struct Winner
{
  Steps_type record_steps;
  Thread_ptr path;
}act_winner = {2000000000, nullptr};

ID_type winning_ID;
const uint64_t n = 12, m = 6;


/******/
Steps_type Positions_table[3 * m * n - m - n];
// 0 for not legal
// 1 for legal
// 2 for winnig
/******/ 

Block_type map[m][n] = 
  {
    {0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 3, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0}
  };

ID_type position_hash(Position position)
{
  ID_type p = (ID_type)position.orientation;
  ID_type x = (ID_type)position.x;
  ID_type y = (ID_type)position.y;

  return (y * (2 * n + p * (p * p - p - 2)) + p * m * (2 * n - p + 1))/2 + x;
}

void check_illegal_positions(void)
{
  Steps_type aux;
  ID_type aux_ID;

  Position aux_position;
  aux_position.orientation = 0;
  for(int y = 0; y < m; y++)
  {
    for(int x = 0; x < n; x++)
    {
      aux_position.x = x;
      aux_position.y = y;

      aux = map[y][x];
      switch(aux){
        case 3 : aux = 0; break;
        case 2 : aux = 1;
      }
      aux_ID = position_hash(aux_position);

      Positions_table[aux_ID] = STEPS_LIMIT_64_BIT * aux;
      if(map[y][x] == (Block_type)2)
        winning_ID = aux_ID;
    }
  }

  aux_position.orientation = 1;
  for(int y = 0; y < m; y++)
  {
    for(int x = 0; x < n - 1; x++)
    {
      aux_position.x = x;
      aux_position.y = y;

      aux = 1;
      if(map[y][x] == 0 || map[y][x + 1] == 0)
        aux = 0;
      Positions_table[position_hash(aux_position)] = aux;
    }
  }

  aux_position.orientation = 2;
  for(int y = 0; y < m - 1; y++)
  {
    for(int x = 0; x < n; x++)
    {
      aux_position.x = x;
      aux_position.y = y;

      aux = 1;
      if(map[y][x] == 0 || map[y + 1][x] == 0)
        aux = 0;
      Positions_table[position_hash(aux_position)] = aux;
    }
  }
}

void move(Thread_ptr thread)
{
  Position aux_position = thread -> where;
  Thread_ptr aux_thread;
  ID_type ID = position_hash(thread -> where);
  if(!Positions_table[ID]){
    return;
  }
  if(thread -> number_steps < Positions_table[ID]){
    Positions_table[ID] = thread -> number_steps;
  }
  else{
    return;
  }

  if(ID == winning_ID){
    act_winner.record_steps = thread -> number_steps;
    act_winner.path         = thread;
    return;
  }

  switch(aux_position.orientation)
  {
    case 0: 
    switch(thread -> move_type){
      case NORTH:
        if(aux_position.x + 1 < n - 1)
          {
            aux_thread = calloc(1, sizeof(Thread));
            aux_thread -> where.orientation = 1;
            aux_thread -> where.x = aux_position.x + 1;
            aux_thread -> where.y = aux_position.y;

            aux_thread -> back_ptr = thread;
            aux_thread -> move_type = EAST;
            aux_thread -> number_steps = thread -> number_steps + 1;
            thread -> step_ptr1 = aux_thread;
            move(aux_thread);
          }
        if(aux_position.x - 2 >= 0)
        {
            aux_thread = calloc(1, sizeof(Thread));
            aux_thread -> where.orientation = 1;
            aux_thread -> where.x = aux_position.x - 2;
            aux_thread -> where.y = aux_position.y;

            aux_thread -> back_ptr = thread;
            aux_thread -> move_type = WEST;
            aux_thread -> number_steps = thread -> number_steps + 1;
            thread -> step_ptr2 = aux_thread;
            move(aux_thread);
        }

        if(aux_position.y - 2 >= 0)
        {
            aux_thread = calloc(1, sizeof(Thread));
            aux_thread -> where.orientation = 2;
            aux_thread -> where.x = aux_position.x;
            aux_thread -> where.y = aux_position.y - 2;

            aux_thread -> back_ptr = thread;
            aux_thread -> move_type = NORTH;
            aux_thread -> number_steps = thread -> number_steps + 1;
            thread -> step_ptr3 = aux_thread;
            move(aux_thread);
        } break;

      case EAST :
      if(aux_position.y + 1 < m - 1)
        {
            aux_thread = calloc(1, sizeof(Thread));
            aux_thread -> where.orientation = 2;
            aux_thread -> where.x = aux_position.x;
            aux_thread -> where.y = aux_position.y + 1;

            aux_thread -> back_ptr = thread;
            aux_thread -> move_type = SOUTH;
            aux_thread -> number_steps = thread -> number_steps + 1;
            thread -> step_ptr1 = aux_thread;
            move(aux_thread);
        }
      if(aux_position.x + 1 < n - 1)
          {
            aux_thread = calloc(1, sizeof(Thread));
            aux_thread -> where.orientation = 1;
            aux_thread -> where.x = aux_position.x + 1;
            aux_thread -> where.y = aux_position.y;

            aux_thread -> back_ptr = thread;
            aux_thread -> move_type = EAST;
            aux_thread -> number_steps = thread -> number_steps + 1;
            thread -> step_ptr1 = aux_thread;
            move(aux_thread);
          }
      if(aux_position.y - 2 >= 0)
        {
            aux_thread = calloc(1, sizeof(Thread));
            aux_thread -> where.orientation = 2;
            aux_thread -> where.x = aux_position.x;
            aux_thread -> where.y = aux_position.y - 2;

            aux_thread -> back_ptr = thread;
            aux_thread -> move_type = NORTH;
            aux_thread -> number_steps = thread -> number_steps + 1;
            thread -> step_ptr3 = aux_thread;
            move(aux_thread);
        } break;
      case SOUTH:
      if(aux_position.y + 1 < m - 1)
        {
            aux_thread = calloc(1, sizeof(Thread));
            aux_thread -> where.orientation = 2;
            aux_thread -> where.x = aux_position.x;
            aux_thread -> where.y = aux_position.y + 1;

            aux_thread -> back_ptr = thread;
            aux_thread -> move_type = SOUTH;
            aux_thread -> number_steps = thread -> number_steps + 1;
            thread -> step_ptr1 = aux_thread;
            move(aux_thread);
        }
      if(aux_position.x + 1 < n - 1)
          {
            aux_thread = calloc(1, sizeof(Thread));
            aux_thread -> where.orientation = 1;
            aux_thread -> where.x = aux_position.x + 1;
            aux_thread -> where.y = aux_position.y;

            aux_thread -> back_ptr = thread;
            aux_thread -> move_type = EAST;
            aux_thread -> number_steps = thread -> number_steps + 1;
            thread -> step_ptr1 = aux_thread;
            move(aux_thread);
          }
      if(aux_position.x - 2 >= 0)
        {
            aux_thread = calloc(1, sizeof(Thread));
            aux_thread -> where.orientation = 1;
            aux_thread -> where.x = aux_position.x - 2;
            aux_thread -> where.y = aux_position.y;

            aux_thread -> back_ptr = thread;
            aux_thread -> move_type = WEST;
            aux_thread -> number_steps = thread -> number_steps + 1;
            thread -> step_ptr2 = aux_thread;
            move(aux_thread);
        } break;
      case WEST:
      if(aux_position.y + 1 < m - 1)
        {
            aux_thread = calloc(1, sizeof(Thread));
            aux_thread -> where.orientation = 2;
            aux_thread -> where.x = aux_position.x;
            aux_thread -> where.y = aux_position.y + 1;

            aux_thread -> back_ptr = thread;
            aux_thread -> move_type = SOUTH;
            aux_thread -> number_steps = thread -> number_steps + 1;
            thread -> step_ptr1 = aux_thread;
            move(aux_thread);

        }
      if(aux_position.x - 2 >= 0)
        {
            aux_thread = calloc(1, sizeof(Thread));
            aux_thread -> where.orientation = 1;
            aux_thread -> where.x = aux_position.x - 2;
            aux_thread -> where.y = aux_position.y;

            aux_thread -> back_ptr = thread;
            aux_thread -> move_type = WEST;
            aux_thread -> number_steps = thread -> number_steps + 1;
            thread -> step_ptr2 = aux_thread;
            move(aux_thread);
        }
      if(aux_position.y - 2 >= 0)
        {
            aux_thread = calloc(1, sizeof(Thread));
            aux_thread -> where.orientation = 2;
            aux_thread -> where.x = aux_position.x;
            aux_thread -> where.y = aux_position.y - 2;

            aux_thread -> back_ptr = thread;
            aux_thread -> move_type = NORTH;
            aux_thread -> number_steps = thread -> number_steps + 1;
            thread -> step_ptr3 = aux_thread;
            move(aux_thread);
        }
    } break;
    case 1:





    // TODO
  


  }
}

int main(void)
{
  check_illegal_positions();

  //ppr si acaso

  return 0;
}
// Posibles posiciones en una 
// cuadrilla de m * n = 3mn - m -n
