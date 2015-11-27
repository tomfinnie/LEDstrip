// LEDlib implementation

//BATTERY LEVEL
//http://www.candlepowerforums.com/vb/showthread.php?81044-Li-Ion-remaining-capacity-vs-voltage
//relies on exact 5v supply for any semblance of accuracy

int calcBatteryPercentage(int batteryRaw)
{
	int batteryPercentage = 0;

	if (batteryRaw > 860) // = 4.2/5 * 1024
	{
		batteryPercentage = 100;
	}
	else if (batteryRaw > 819) // = 4/5 * 1024
	{
		batteryPercentage = 80 + ((batteryRaw - 819) / 2);
	} 
	else
	{
		batteryPercentage = batteryRaw - 739;
	}

	return batteryPercentage;
}