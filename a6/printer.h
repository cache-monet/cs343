#ifndef PRINTER_H
#define PRINTER_H

_Monitor Printer {
  public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    void print( Kind kind, char state );
    void print( Kind kind, char state, unsigned int value1 );
    void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
    void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );
    ~Printer();
  private:
    unsigned int numStudents;
    unsigned int numVendingMachines;
    unsigned int numCouriers;

    struct State {
      Kind kind;
      char state;
      unsigned int lid;
      unsigned int value1;
      unsigned int value2;
      bool empty = true;
    };

    State* buffer; // stores the out for each component
    void flush(); // print all content
    int getIndex(Kind kind, unsigned int lid = 0);
};
#endif