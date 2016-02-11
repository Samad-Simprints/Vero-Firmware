package com.simprints.scanner.library;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class Scanner implements ConnectionCallback
{
  private Connection connection;
  private ScannerCallback callback;

  private static final int ERROR_BASE = 0;
  public static final int ERROR_NONE = ERROR_BASE;

  // values must match those in firmware msg_format.h
  public static final byte MSG_GET_SENSOR_INFO   = 0;
  public static final byte MSG_SET_UI            = 2;
  public static final byte MSG_REPORT_UI         = 4;
  public static final byte MSG_CAPTURE_IMAGE     = 5;
  public static final byte MSG_RECOVER_IMAGE     = 8;
  public static final byte MSG_GENERATE_TEMPLATE = 9;
  public static final byte MSG_RECOVER_TEMPLATE  = 10;
  public static final byte MSG_IMAGE_QUALITY     = 11;
  public static final byte MSG_UN20_SHUTDOWN     = 14;
  public static final byte MSG_UN20_WAKEUP       = 15;

  public static final byte MSG_REPLY             = (byte)0x80;

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

    public boolean boEnableTrigger;
    public boolean boSetLeds;
    public boolean boTriggerVibrate;
    public byte[] bLedState; // size is fixed at LED_MAX_COUNT by constructor
    public short iVibrateMs;

    // constructor
    public UIControl()
    {
      bLedState = new byte[LED_MAX_COUNT];
    }
  }

  @Override
  public void onMessageReceived(Message msg)
  {

  }

  private void writeMessage(Message msg)
  {
    connection.writeMessage(msg.buffer.getShort(4), msg.buffer.array());
  }

  // returns a message read in two passes
  // the first reads the fixed part: header and length
  // the second reads the rest based on the length field
  private Message readMessage()
  {
    int iLenFixedPart = Message.MSG_ID_OFFSET;
    ByteBuffer fixedPart = ByteBuffer.allocate(iLenFixedPart);
    fixedPart.order(ByteOrder.LITTLE_ENDIAN);

    connection.readMessage(iLenFixedPart,fixedPart.array());

    short iLength = fixedPart.getShort(Message.INT_SIZE);
    Message rply = new Message(iLength - Message.MSG_OVERHEAD);

    rply.buffer.put(fixedPart.array());
    connection.readMessage(iLength - iLenFixedPart, rply.buffer.array(), Message.MSG_ID_OFFSET);

    return rply;
  }

  // constructor
  public Scanner(Connection connection, ScannerCallback callback)
  {
    this.connection = connection;
    this.callback = callback;
  }

  public boolean open()
  {
    return this.connection.open(this);
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
    Message msg = new Message(9);
    msg.setTxHeader(MSG_CAPTURE_IMAGE);

    msg.buffer.put((byte) 0); // false
    msg.buffer.putShort((short) 0); // timeout
    msg.buffer.putShort((short) 0); // retry count
    msg.buffer.putShort((short)50); // quality
    msg.buffer.putShort((short) 50); // brightness

    writeMessage(msg);
    Message rply = readMessage();
    callback.onStatusChange("image:scan complete");

    return ERROR_NONE;
  }

  public Image readImage()
  {
    Message msg = new Message(4);
    msg.setTxHeader(MSG_RECOVER_IMAGE);

    writeMessage(msg);
    Message rply = readMessage();
    callback.onStatusChange("image:transfer complete");

    return new Image();
  }

  public int getTemplate() {
    Message msg = new Message(4);
    msg.setTxHeader(MSG_GENERATE_TEMPLATE);

    writeMessage(msg);
    Message rply = readMessage();
    callback.onStatusChange("template:conversion complete");

    return ERROR_NONE;
  }

  public Template readTemplate() {
    Template template = new Template();
    Message msg = new Message(4);
    msg.setTxHeader(MSG_RECOVER_TEMPLATE);

    writeMessage(msg);
    Message rply = readMessage();
    callback.onStatusChange("template:transfer complete");

    return template;
  }

  public int getImageQuality() {
    byte bImageQuality = 0;
    Message msg = new Message(0);
    msg.setTxHeader(MSG_IMAGE_QUALITY);

    writeMessage(msg);
    Message rply = readMessage();

    return bImageQuality;
  }

  public int getBatteryPercent()
  {
    short BatteryLevel1 = 0;
    Message msg = new Message(0);
    msg.setTxHeader(MSG_GET_SENSOR_INFO);
    writeMessage(msg);

    Message rply = readMessage();
    rply.buffer.position(Message.MSG_ID_OFFSET);

    byte bMsgId = rply.buffer.get();
    byte bStatus = rply.buffer.get();

    if (bMsgId==(MSG_GET_SENSOR_INFO + MSG_REPLY))
    {
      byte[] bdaddr = new byte[6];
      rply.buffer.get(bdaddr);
      short UCVersion = rply.buffer.getShort();
      short UNVersion = rply.buffer.getShort();
      BatteryLevel1 = rply.buffer.getShort();
      short BatteryLevel2 = rply.buffer.getShort();
      short StoreCount = rply.buffer.getShort();
      boolean boPowerOn = rply.buffer.get() != 0;
    }

    return BatteryLevel1;
  }

  public int setUI(UIControl uicontrol)
  {
    Message msg = new Message(1 + 1 + 1 + UIControl.LED_MAX_COUNT + 2);
    msg.setTxHeader(MSG_SET_UI);

    msg.buffer.put((byte) (uicontrol.boEnableTrigger ? 1 : 0));
    msg.buffer.put((byte) (uicontrol.boSetLeds? 1: 0));
    msg.buffer.put((byte) (uicontrol.boTriggerVibrate ? 1 : 0));
    msg.buffer.put(uicontrol.bLedState);
    msg.buffer.putShort(uicontrol.iVibrateMs);

    writeMessage(msg);

    // Message rply = readMessage();

    return ERROR_NONE;
  }

  public UIControl getUI()
  {
    Message msg = new Message(4);
    msg.setTxHeader(MSG_REPORT_UI);

    writeMessage(msg);
    Message rply = readMessage();

    return new UIControl();
  }

  public int setUn20bPower(boolean poweron)
  {
    Message msg = new Message(4);
    msg.setTxHeader(poweron? MSG_UN20_WAKEUP: MSG_UN20_SHUTDOWN);

    writeMessage(msg);
    Message rply = readMessage();

    return ERROR_NONE;
  }

  public int setButton(boolean enabled)
  {
    Message msg = new Message(4);
    msg.setTxHeader(MSG_SET_UI);

    writeMessage(msg);
    Message rply = readMessage();

    return ERROR_NONE;
  }
}
