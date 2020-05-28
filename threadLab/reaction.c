//
// Matthew Imiolek
//

#include "thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
	// Lock structure
	struct lock lock;
	// Condition structure used to check if there are H atoms present
  struct condition hpresent;
	// Condition structure used to check if there are O atoms present
  struct condition opresent;
	// Integer holding the number of H atoms that can be used in making water
  int hs;
	// Integer holding the number of O atoms that can be used in making water
  int os;
};

// Initializes all of the structures and values used by later functions for
// locking
void
reaction_init(struct reaction *reaction)
{
  lock_init(&reaction->lock);
	cond_init(&reaction->hpresent);
	cond_init(&reaction->opresent);
	reaction->hs = 0;
	reaction->os = 0;
}

// Function called whenever an H atom thread runs, preparing an H atom for use
// in making water
void
reaction_h(struct reaction *reaction)
{
	lock_acquire(&reaction->lock);

	// Increase the number of H atoms available to use in making water
  reaction->hs++;

	// When there are more than 1 H atoms that can be used, signal an O atom
	// thread that it can continue and make water
	if (reaction->hs > 1)
		cond_signal(&reaction->hpresent, &reaction->lock);

	// Wait until there is an O atom present to make water
	cond_wait(&reaction->opresent, &reaction->lock);

  lock_release(&reaction->lock);
}

// Function called whenever an O atom thread runs, preparing an O atom for use
// in making water, which this function also does
void
reaction_o(struct reaction *reaction)
{
  lock_acquire(&reaction->lock);

	// Increase the number of O atoms available to use in making water
  reaction->os++;

	// When there are less than 2 H atoms that can be used, wait for more H atoms
	while (reaction->hs < 2)
    cond_wait(&reaction->hpresent, &reaction->lock);

	// Decrease the number of O and H atoms that are available for use (done here
  // as this is where water is actually made)
	reaction->os--;
	reaction->hs -= 2;

	// Signal two H atom threads that they are needed to make water
	cond_signal(&reaction->opresent, &reaction->lock);
	cond_signal(&reaction->opresent, &reaction->lock);

	// Make water
	make_water();

	lock_release(&reaction->lock);
}
