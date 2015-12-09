package com.simprints.scanner.library;

import android.util.Log;

/**
 * Created by derek on 18/08/2015.
 */
public class Scanner
{
  private Connection connection;

  private static final byte CMD_BASE = 1;
  public static final byte CMD_IMAGE       = CMD_BASE + 1;
  public static final byte CMD_TEMPLATE    = CMD_BASE + 2;
  public static final byte CMD_GET_PARAMS  = CMD_BASE + 3;
  public static final byte CMD_SET_PARAMS  = CMD_BASE + 4;
  public static final byte CMD_STORE_IMAGE = CMD_BASE + 5;
  public static final byte CMD_FETCH_IMAGE = CMD_BASE + 6;
  public static final byte CMD_BATTERY     = CMD_BASE + 7;
  public static final byte CMD_LEDS        = CMD_BASE + 8;

  public class ScanParameters
  {
    public int quality;
    public int contrast;
    public int brightness;
    public int gain;

    public ScanParameters(int quality,int contrast,int brightness,int gain)
    {
      this.quality = quality;
      this.contrast = contrast;
      this.brightness = brightness;
      this.gain = gain;
    }
  }

  public class ImageHandle
  {
    public int handle;
  }

  public Scanner(Connection connection) {
    this.connection = connection;
  }

  public Image getImage()
  {
    Image image = new Image();

    connection.writeCommand(CMD_IMAGE, 0, null);
    // connection.read(image.getData());

    return image;
  }

  public Template getTemplate() {
    Template template = new Template();

    connection.writeCommand(CMD_TEMPLATE, 0, null);
    // connection.readResponse();

    return template;
  }

  public ImageHandle storeImage()
  {
    ImageHandle imageHandle = new ImageHandle();

    connection.writeCommand(CMD_STORE_IMAGE,0,null);
    imageHandle.handle = 0;

    return imageHandle;
  }

  public Image fetchImage(ImageHandle handle)
  {
    connection.writeCommand(CMD_FETCH_IMAGE, 0, null);
    return new Image();
  }

  public void setParameters(ScanParameters parameters)
  {
    connection.writeCommand(CMD_GET_PARAMS, 0, null);
  }

  public ScanParameters getParameters()
  {
    connection.writeCommand(CMD_SET_PARAMS, 0, null);

    return new ScanParameters(50,50,50,50);
  }

  public int getBatteryPercent()
  {
    connection.writeCommand(CMD_BATTERY,0,null);

    return 50;
  }

  public void setLeds()
  {
    connection.writeCommand(CMD_LEDS,0,null);
  }
}
