// COMPILE COMMANDS:
// $ u++ -g -multi schmilblick.cc -DCFOR
// $ u++ -g -multi schmilblick.cc -DACTOR
// $ u++ -g -multi schmilblick.cc -DTASK

/* SAMPLE TEST DATA, 2 MATRICES
-1 5 5
1 -1 3 4 -1
2 1 4 -1 6
3 -1 1 6 -1
4 -1 6 1 8
-1 6 7 -1 1

5 5 5
1 2 3 4 5
2 1 4 5 6
3 4 1 6 7
4 5 6 1 8
5 6 7 8 1
*/

// Add extra call to this routine after deleting a task
// uEHM::poll(); // poll for async exception

#if defined( CFOR )
#include <uCobegin.h>
#elif defined( ACTOR )
#include <uActor.h>
#endif
#include <iostream>
using namespace std;

bool schmilblickCheck( const int row[], int cols, int schmilblick ) {
    int count = 0;
    for (int i = 0; i < cols; i++) { // go through the row and count sch
        if (row[i] == schmilblick) count++;
    }
    return count >= 2;
} // schmilblickCheck

#if defined( ACTOR )

struct WorkMsg : public uActor::Message {
    int * const row;
    int cols;
    int schmilblick;
    bool& found; // comnicate found to main by reference
    WorkMsg(int * const row, int cols, int schmilblick, bool& found):
        Message(uActor::Delete), row(row), cols(cols), schmilblick(schmilblick), found(found) {};
    // WRITE THIS TYPE
}; // WorkMsg

_Actor Schmilblick {
    Allocation receive( Message & msg ) {
	// WRITE THIS MEMBER
        Case( WorkMsg, msg ) {
            if (schmilblickCheck(msg_d->row, msg_d->cols, msg_d->schmilblick)) msg_d->found = true; // set found to true
        };
        return Finished;
    } // Schmilblick::receive
}; // Schmilblick

#elif defined( TASK )

_Task Schmilblick {                                     // check row of matrix
    int r;
    const int * row;
    int cols;
    uBaseTask & pgmMain;
    int schmilblick;
    void main() {
        try {
            _Enable { // enable catching stop
                yield( rand() % 100 );				// random task starts
                if (schmilblickCheck(row, cols, schmilblick)) {
                    _Resume SCHMILBLICK() _At pgmMain;
                    return;
                }
            } // Enable
        } catch (Stop& ) {
            cout << "told to stop " << r << endl;
            return;
        } // try
    } // Schmilblicks::main
  public:
    _Event Stop {};                                     // concurrent exceptions
    _Event SCHMILBLICK {};
    // YOU WRITE THIS MEMBER
    Schmilblick( int r, const int row[], int cols, uBaseTask & pgmMain, int schmilblick ) :
        r(r), row(row), cols(cols), pgmMain(pgmMain), schmilblick(schmilblick) {};
}; // Schmilblick
#endif

int main() {
    srand( getpid() );

    int schmilblick, rows, cols;
    cin >> schmilblick >> rows >> cols;
    int M[100][100], r, c;
    bool found = false;

    for ( r = 0; r < rows; r += 1 ) {                   // read/print matrix
        for ( c = 0; c < cols; c += 1 ) {
            cin >> M[r][c];
            cout << M[r][c] << ", ";
        } // for
        cout << endl;
    } // for
    cout << endl;

    uProcessor p;

#if defined( CFOR )
COFOR(r,0, rows, 
   if (schmilblickCheck(M[r], cols, schmilblick)) found = true;
);
#elif defined( ACTOR )
    uActorStart();
    Schmilblick workers[rows];
    for (int r = 0; r < rows; r++) workers[r] | *new WorkMsg(M[r], cols, schmilblick, found); // actors
    uActorStop();
#elif defined( TASK )
    Schmilblick* workers[rows]; // create 

    for (r = 0; r < rows; r++) workers[r] = new Schmilblick(r, M[r], cols, uThisTask(), schmilblick); // create tasks
    int s = 0; // keeps track of deleted tasks
    try {
        _Enable { // enable catch SCHMILBLICKs
            for (; s < rows; s++) {
                delete workers[s];
                uEHM::poll(); // async
            }
        }  // Enable
    } _CatchResume(Schmilblick::SCHMILBLICK& ) {
        if (!found) { // stop all running task
            // i = s+1 in order to not stop already deleted task
            for (int i = s+1; i < rows; i++) _Resume Schmilblick::Stop() _At *workers[i]; // stop worker
        } // if
        found = true;
    } // try
    // YOU WRITE THIS CODE
#else
	#error unknown concurrency approach specified on command line, e.g., need -DTASK
#endif

    cout << "Schmilblicks" << (found ? " " : " not ") << "found" << endl;
} // main
