//
// Matthew Imiolek
//

#include "thread.h"

struct station {
	// Lock structure
	struct lock lock;
	// Condition structure used to check if a train is at the station
	struct condition trainpresent;
	// Conditon structure used to check if the train is full
	struct condition trainfull;
	// Integer holding the number of seats left on the train
	int available;
	// Integer holding the number of people waiting for seats
	int waiting;
	// Integer holding the number of people on the train who have not sat
	int standing;
};

// Initializes all of the structures and values used by later functions for
// locking
void
station_init(struct station *station)
{
	lock_init(&station->lock);
	cond_init(&station->trainpresent);
	cond_init(&station->trainfull);
	station->available = 0;
	station->waiting = 0;
	station->standing = 0;

}

// Function which waits until a train is fully loaded before returning
void
station_load_train(struct station *station, int count)
{
	lock_acquire(&station->lock);

	// Sets the number of available seats to the given number
	station->available = count;

	// Broadcasts to all other threads that a train has arrived at the station
	cond_broadcast(&station->trainpresent, &station->lock);

	// Waits while there are passengers standing, or if there are both available
	// seats and people waiting for those seats
	while (station->standing > 0 ||
					(station->available > 0 && station->waiting > 0))
		cond_wait(&station->trainfull, &station->lock);

	// Sets the available seats to 0 to avoid post full train attempts at boarding
	station->available = 0;
	lock_release(&station->lock);
}

// Function where passengers wait until a train arrives at the station and they
// can board that train
void
station_wait_for_train(struct station *station)
{
	lock_acquire(&station->lock);

	// Increase the number of passengers waiting for the train
	station->waiting++;

	// Waits while there is no train present
	while (station->available == 0)
		cond_wait(&station->trainpresent, &station->lock);

	// Once a train arrives, decrease the number of available seats (the current
  // passanger will take one), decrease the number of waiting passengers, and
	// increase the number of standing passengers (the current passeneger is now
  // standing)
	station->available--;
	station->waiting--;
	station->standing++;
	lock_release(&station->lock);
}

// Function where the passenger lets the train know that a passenger has sat
// down
void
station_on_board(struct station *station)
{
	lock_acquire(&station->lock);

	// Decrease the number of standing passengers
	station->standing--;

	// If there are no standing passengers, and there are either no more seats
	// available on the train or there are no more passengers waiting for the
	// train, signal that the train is full and can leave
	if(station->standing == 0 && (station->available == 0 || station->waiting == 0))
		cond_signal(&station->trainfull, &station->lock);
	lock_release(&station->lock);
}
