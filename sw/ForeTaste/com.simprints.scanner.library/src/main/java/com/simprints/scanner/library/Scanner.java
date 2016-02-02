package com.simprints.scanner.library;

public class Scanner
{
  private Connection connection;
  private ScannerCallback callback;

  private static final int ERROR_BASE = 0;
  public static final int ERROR_NONE = ERROR_BASE;

  private static final byte CMD_BASE = 0;
  public static final byte CMD_IMAGE       = CMD_BASE;
  public static final byte CMD_GET_IMAGE   = CMD_BASE + 1;
  public static final byte CMD_TEMPLATE    = CMD_BASE + 2;
  public static final byte CMD_GET_TEMPLATE = CMD_BASE + 3;
  public static final byte CMD_QUALITY     = CMD_BASE + 4;
  public static final byte CMD_POWER       = CMD_BASE + 5;
  public static final byte CMD_BUTTON      = CMD_BASE + 6;
  public static final byte CMD_BATTERY     = CMD_BASE + 7;
  public static final byte CMD_SET_UI      = CMD_BASE + 8;
  public static final byte CMD_GET_UI      = CMD_BASE + 9;

  public class UIControl
  {
    public static final int LED_RING_0 = 0;
    public static final int LED_RING_1 = 1;
    public static final int LED_RING_2 = 2;
    public static final int LED_RING_3 = 3;
    public static final int LED_RING_4 = 4;
    public static final int LED_RING_5 = 5;
    public static final int LED_RING_6 = 6;
    public static final int LED_GOOD_SCAN_FEEDBACK = 7;
    public static final int LED_BAD_SCAN_FEEDBACK = 8;
    public static final int LED_CONNECTION = 9;
    public static final int LED_BATTERY = 10;

    public static final int LED_MAX_COUNT = 11;

    public static final byte LED_STATE_OFF = 0;
    public static final byte LED_STATE_RED = 1;
    public static final byte LED_STATE_GREEN = 2;
    public static final byte LED_STATE_ORANGE = 3;
    public static final byte LED_STATE_ON = 4;

    public boolean boSetLeds;
    public boolean boTriggerVibrate;
    byte[] bLedState; // size is fixed at LED_MAX_COUNT by constructor
    int iVibrateMs;

    public UIControl()
    {
      bLedState = new byte[LED_MAX_COUNT];
    }
  }

  // constructor
  public Scanner(Connection connection, ScannerCallback callback)
  {
    this.connection = connection;
    connection.open();
    this.callback = callback;
  }

  public void close()
  {
    if (this.connection!=null)
    {
      connection.close();
      this.connection = null;
    }
  }

  public int getImage()
  {
    connection.writeCommand(CMD_GET_IMAGE, 0, null);
    callback.onStatusChange("image:scan complete");

    return ERROR_NONE;
  }

  public Image readImage()
  {
    connection.writeCommand(CMD_IMAGE, 0, null);
    callback.onStatusChange("image:transfer complete");

    return new Image();
  }

  public int getTemplate() {
    connection.writeCommand(CMD_GET_TEMPLATE, 0, null);
    callback.onStatusChange("template:conversion complete");

    return ERROR_NONE;
  }

  public Template readTemplate() {
    Template template = new Template();
    connection.writeCommand(CMD_TEMPLATE, 0, null);
    callback.onStatusChange("template:transfer complete");

    return template;
  }

  public int getImageQuality() {
    byte[] bImageQuality = new byte[1];
    connection.writeCommand(CMD_QUALITY, 0, null);
    connection.readResponse(1, bImageQuality);

    return bImageQuality[0];
  }

  public int getBatteryPercent()
  {
    byte[] bBatteryLevel = new byte[1];
    connection.writeCommand(CMD_BATTERY,0,null);
    connection.readResponse(1,bBatteryLevel);

    return bBatteryLevel[0];
  }

  public int setUI(UIControl uicontrol)
  {
    connection.writeCommand(CMD_SET_UI,0,null);

    return ERROR_NONE;
  }

  public UIControl getUI()
  {
    connection.writeCommand(CMD_GET_UI,0,null);

    return new UIControl();
  }

  public int setUn20bPower(boolean poweron)
  {
    connection.writeCommand(CMD_POWER,0,null);

    return ERROR_NONE;
  }

  public int setButton(boolean enabled)
  {
    connection.writeCommand(CMD_BUTTON,0,null);

    return ERROR_NONE;
  }
}
