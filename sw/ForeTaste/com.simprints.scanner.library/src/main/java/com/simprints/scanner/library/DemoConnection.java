package com.simprints.scanner.library;

import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Created by derek on 24/08/2015.
 */
public class DemoConnection extends Connection
{
  private static final String TAG = "DemoConnect";
  public DemoConnection()
  {
    super("Demonstration");
  }
  private ByteBuffer reply = null;

  public boolean init() {
    isSetup = true;
    return true;
  }

  public String deviceName()
  {
    return "Demo Dev";
  }

  public String deviceDetail() {
    return "Demo Device Details";
  }

  // override only used here to add Logging
  // although it would be more extensive in an actual connection
  @Override
  public void open(ConnectionCallback callback)
  {
    super.open(callback);
    Log.v(TAG,"open");
  }

  public void close() 
  {
    Log.v(TAG,"close");
  }

  // set reply to provided length
  // fill in known fields
  private void setReply(int length,byte bCmd)
  {
    short iLength = (short)(length + Message.MSG_OVERHEAD);
    reply = ByteBuffer.allocate(iLength);
    reply.order(ByteOrder.LITTLE_ENDIAN);

    reply.putInt(Message.MSG_HEADER);
    reply.putShort(iLength);
    reply.put((byte) (bCmd + Scanner.MSG_REPLY));
    reply.put((byte)0); //status

    reply.putInt(iLength - Message.INT_SIZE,Message.MSG_FOOTER);
  }

  public void writeMessage(int length, byte[] data)
  {
    byte bCmd = data[6];
    Log.v(TAG,"writeMessage " + length + " bytes");

    switch(bCmd)
    {
      case Scanner.MSG_GET_SENSOR_INFO:
        Log.v(TAG, "writeMessage MSG_GET_SENSOR_INFO");

        setReply(6 + (5*2) + 1,bCmd);
        byte[] bdAddr = new byte[] {1,2,3,4,5,6};
        reply.put(bdAddr);
        reply.putShort((short) 1);
        reply.putShort((short) 3);
        reply.putShort((short)50);
        reply.putShort((short)50);
        reply.putShort((short)0);
        reply.put((byte) 0);

        reply.position(0);
        break;

      case Scanner.MSG_SET_UI:
        Log.v(TAG, "writeMessage MSG_SET_UI");
        break;

      case Scanner.MSG_REPORT_UI:
        Log.v(TAG,"writeMessage MSG_REPORT_UI");
        break;

      case Scanner.MSG_CAPTURE_IMAGE:
        Log.v(TAG,"writeMessage MSG_CAPTURE_IMAGE");
        break;

      case Scanner.MSG_RECOVER_IMAGE:
        Log.v(TAG,"writeMessage MSG_RECOVER_IMAGE");
        break;

      case Scanner.MSG_GENERATE_TEMPLATE:
        Log.v(TAG,"writeMessage MSG_GENERATE_TEMPLATE");
      break;

      case Scanner.MSG_RECOVER_TEMPLATE:
        Log.v(TAG,"writeMessage MSG_RECOVER_TEMPLATE");
        break;

      case Scanner.MSG_IMAGE_QUALITY:
        Log.v(TAG,"writeMessage MSG_IMAGE_QUALITY");
        break;

      case Scanner.MSG_UN20_SHUTDOWN:
        Log.v(TAG,"writeMessage MSG_UN20_SHUTDOWN");
        break;

      case Scanner.MSG_UN20_WAKEUP:
        Log.v(TAG,"writeMessage MSG_UN20_WAKEUP");
        break;
    }
  }

  public void readMessage(int length, byte[] data, int offset)
  {
    int i;

    for ( i = offset; i<(length + offset); ++i )
    {
      data[i] = reply.get();
    }
  }

}
