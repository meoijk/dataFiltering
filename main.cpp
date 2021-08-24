// Author: Marcelo Elias de Oliveira

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <iomanip>
#include <algorithm>

//using namespace std;

// In the ideal case we would implement a Kalman-filter class, however, this may not be the focus of this exercise
// As an example we implemented the moving average and exponential moving average filters,
// where the later is derived from a RC low-pass filter circuit
enum FilteringMethodEnum { MAV, EMA, KF };

// Defining synthetic gazes for eye-tracking
// In this example, we are assuming 2D data only, however, it could also be 3D if stereo-vision is assumed
enum DataEnum { SIN = 1, COS };


template<typename T>
std::vector<T*> SynDataGen(DataEnum dataE, T freq, T time, T dt, int numOfChannels, T SNR = 0.2)
{
    // Channel [0]:  time clock data
    numOfChannels+= 1;

    std::vector<T*> rawData;
    //T dt = time / static_cast<T>(numOfPts);
    long int numOfPts = static_cast<T>( ceil(time / dt) );

    for (int i = 0; i < numOfPts; ++i)
    {        
        T* xy = new float[numOfChannels];
        xy[0] = static_cast<T>( i * dt );
        for (int j = 1; j < numOfChannels; ++j)    
        {
            T noise = (static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX) - 0.5) * SNR;

            if (dataE == DataEnum::SIN)
            {                
                xy[j] = static_cast<T>( sin( 2.0 * M_PI * freq * i * dt ) ) + noise;                
            }
            else if (dataE == DataEnum::COS)
            {                
                xy[j] = static_cast<T>( cos( 2.0 * M_PI * freq * i * dt ) ) + noise;                
            }            
        }
        rawData.push_back(xy);                        
        //std::cout << rawData.at(i)[0] << "\t" << rawData.at(i)[1] << std::endl;        
    }
    return rawData;
}


template<typename T>
std::vector<T*> FilteringData(FilteringMethodEnum filtMethod, std::vector<T*> rawData, int numOfChannels, int winSize, T alpha = 0.1)
{
    reverse(rawData.begin(), rawData.end());
    // Channel [0]:  time clock data
    numOfChannels+= 1;

    std::vector<T*> filtData;

    for (typename std::vector<T*>::iterator it = rawData.begin(); it != rawData.end(); ++it)    
    {   
        long int idx = std::distance(rawData.begin(), it);
        T* xy = new T[numOfChannels];
        xy[0] = (*it)[0];

        for (int j = 1; j < numOfChannels; ++j)
        {
            if (filtMethod == FilteringMethodEnum::MAV)
            {   
                if (idx < 1) xy[j] = (*it)[j];
                else if (idx < winSize)
                {
                    xy[j] = ( filtData.at(idx-1)[j] * static_cast<T>(idx) + (*it)[j] ) / static_cast<T>( idx + 1 );
                } 
                else
                {
                    xy[j] = ( filtData.at(idx-1)[j] * static_cast<T>(winSize) + (*it)[j] - rawData.at(idx-winSize)[j] ) / static_cast<T>( winSize );
                }                    
            }
            else if (filtMethod == FilteringMethodEnum::EMA)
            {
                // Derived from a low-pass RC filter
                // alpha = dt / (dt + RC)
                if (idx < 1) xy[j] = alpha * (*it)[j];
                else
                {
                    xy[j] = alpha * (*it)[j] + (1.0 - alpha) * filtData.at(idx-1)[j];
                }
            }
            else if (filtMethod == FilteringMethodEnum::KF)
            {   
                std::cerr << filtMethod << ": Not implemented in this example\n" << std::endl;
                exit(EXIT_FAILURE);
            }
        }        
        filtData.push_back(xy);
        //std::cout << filtData.at(idx)[0] << "\t" << filtData.at(idx)[1] << std::endl;        
    }
    return filtData;
}

template<typename T>
int WriteDataToFile(std::vector<T*> data, const char* fileName, int numOfChannels)
{
    // Channel [0]:  time clock data
    numOfChannels+= 1;

    std::ofstream outFile(fileName);

    if (!outFile.is_open()) 
    {
        std::cerr << "Could not write to output file" << std::endl;
        EXIT_FAILURE;
    }

    outFile << std::fixed;
    outFile << std::setprecision(6);
    for (typename std::vector<T*>::iterator it = data.begin(); it != data.end(); ++it )
    {        
        for (int j = 0; j < numOfChannels; ++j) outFile << (*it)[j] << "\t";        
        outFile << "\n";
    }        
    return EXIT_SUCCESS;
}


int main(int argc, char*argv[])
{
    if (argc < 3) 
    {     
        std::cerr << "Usage: " << argv[0] << " PATH/syntheticRawDataNAME.txt" << " PATH/filteredDataNAME.txt" << std::endl;      
        return EXIT_FAILURE;
    }

    // Synthetic data generation    
    // Freq: 5 Hz | Period: 1s | SNR: 0.5 | dt: 0.001 | numOfChannels: 2
    int numOfChannels = 2;
    std::vector<float*> rawData = SynDataGen<float>(DataEnum::SIN, 5.0, 1.0, 0.001, numOfChannels, 0.5);

    // Data filtering
    // Window size: 20
    // α := dt / (RC + dt)

    FilteringMethodEnum filteringMethod = FilteringMethodEnum::MAV;
    std::vector<float*> filtData = FilteringData<float>(filteringMethod, rawData, numOfChannels, 20, 0.07);
    // Filt-filt data
    filtData = FilteringData<float>(filteringMethod, filtData, numOfChannels, 20, 0.07);
    
    // Writing data to file    
    WriteDataToFile<float>(rawData, argv[1], numOfChannels);
    WriteDataToFile<float>(filtData, argv[2], numOfChannels);    
    
    return EXIT_SUCCESS;
}
