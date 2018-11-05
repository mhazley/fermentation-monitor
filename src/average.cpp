#include "average.h"
#include "fermenter.h"


Average::Average()
{
    // Empty Constructor
}

double Average::getAverage()
{
    return this->average;
}

void Average::addValue(  double value  )
{
    this->values[this->curIndex] = value;

    this->curIndex++;

    if( this->curIndex >= AVERAGE_SIZE  )
    {
        this->curIndex = 0;
    }
    
    this->calculateAverage();
}

void Average::calculateAverage()
{
    int i;
    double sum = 0;
    
    Serial.printf("AVERAGE: ");
    
    for( i = 0; i < AVERAGE_SIZE; i++ )
    {
        sum += this->values[i];
        Serial.printf("%f, ", this->values[i]);
    }
    
    this->average = ( sum / (double)AVERAGE_SIZE );
    Serial.printf("%f\n", this->average);
}
