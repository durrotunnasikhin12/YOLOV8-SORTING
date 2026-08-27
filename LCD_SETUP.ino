void setupLCD()
{
    lcd.init();
    lcd.backlight();

    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("YOLO SORTING");

    lcd.setCursor(0,1);
    lcd.print("Initializing");

    delay(1500);

    updateLCD();
}

void updateLCD()
{
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print(lastDetection);

    lcd.setCursor(0,1);

    lcd.print("N:");
    lcd.print(countNut);

    lcd.print(" B:");
    lcd.print(countBolt);

    lcd.print(" Br:");
    lcd.print(countBearing);
}