/*! 
@file 
@brief 
@details 

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
#ifndef FILTERS_H
#define FILTERS_H
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structs
 ******************************************************************************/
/*! @brief structure representing moving average data */
typedef struct
{
	/*! @brief mean observed in samples */
	float avg;
	/*! @brief if the average is not for full count this will be false */
	bool stable;
	/*! @brief pointer to the data array */
	float* dataPtr;
	/*! @brief no of samples per computation */
	const int count;
	/*! @brief initialize to zero - used internally */
	int index;
} mov_avg_float_t;
 /*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
@brief Evaluates the moving average for float data
@param *data- averaging structure
@param val- current value
*/
static float MovingAverage_Float_Evaluate(mov_avg_float_t* data, float val)
{
	data->dataPtr[data->index] = val;					/* add new data */
	data->avg = 0;
	if(data->stable == false)									/* data is considered stable if the whole array is finished atleast once */
	{
		data->index++;
		
		for (int i = 0; i < data->index; i++)		/* average will only contain data till current index */
			data->avg += data->dataPtr[i];
		data->avg /= data->index;
		
		if(data->index >= data->count)					/* if data completed mark the data stable */
		{
			data->stable = true;
			data->index = 0;
		}
	}
	else																			/* data is stable compute average over the whole data */
	{
		for (int i = 0; i < data->count; i++)
			data->avg += data->dataPtr[i];
		data->avg /= data->count;
		if(++data->index >= data->count)
			data->index = 0;
	}
	return data->avg;
}

/*!
@brief Resets the moving average for float data
@param *data- averaging structure
*/
static void MovingAverage_Float_Reset(mov_avg_float_t* data)
{
	data->avg = 0;
	data->index = 0;
	for(int i = 0; i < data->count; i++)
		data->dataPtr[i] = 0;
	data->stable = false;
}

#endif
/* EOF */
