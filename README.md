# dataFiltering

** Implemented methods **

In this example, two basic techniques for data filtering were implemented, i.e., moving and exponential averages, where the latter is derived from a low-pass RC circuit. These methods are efficient and adequate for real-time data filtering since only the present state is used to compute the filtered data.

enum FilteringMethodEnum { MAV, EMA, KF }

In main.cpp, only native C++ libraries are considered, and for this reason, a simple implementation of a Kalman filter in C# is provided. For more details, please refer to kalmanFiltering.cs

2D synthetic data with external artifacts were generated. However, the developed functions support higher-dimensional data. 

enum DataEnum { SIN = 1, COS }


** Synthetical data generation **

std::vector<TYPE*> data = SynDataGen<TYPE>(DataEnum::TYPE, waveFreq, time, sample interval, numOfChannels, signal-to-noise ratio)

** Filtered data **

std::vector<TYPE*> filtData = FilteringData<TYPE>(Filtering_METHOD, rawData, numOfChannels, winSizeparameter, OptionalParameterRCFilterDerived)

** Write data to file: **

WriteDataToFile<TYPE>(data, path, numOfChannels);


![MAV method](https://github.com/meoijk/dataFiltering/blob/main/doc/MAV_method.png?raw=true)



![EMA method](https://github.com/meoijk/dataFiltering/blob/main/doc/EMA_method.png?raw=true)
