package com.simprints.scanner.library;

import java.util.ArrayList;

/**
 * Created by derek on 21/08/2015.
 */

/* singleton library which contains the Connection instances
*   use getInstance rather than constructor, which is private to prevent that happening
* */
public class ScannerLib
{
  private static ScannerLib ourInstance = new ScannerLib();

  // connection methods
  public static final int METHOD_USB = 0;
  public static final int METHOD_BT = 1;
  public static final int METHOD_BLE = 2;
  public static final int METHOD_WIFI = 3;
  public static final int METHOD_AJ = 4;

  // only USB and BT are implemented
  public static final int METHOD_COUNT = 5;

  // arrayList of Connections, indexed by method
  private ArrayList<Connection> connections = new ArrayList<Connection>(METHOD_COUNT);

  // singleton "constructor"
  public static ScannerLib getInstance()
  {
    return ourInstance;
  }

  // private constructor
  private ScannerLib()
  {
    connections.add(METHOD_USB,new Connection(METHOD_USB,"USB"));
    connections.add(METHOD_BT,new Connection(METHOD_BT,"Bluetooth"));
    connections.add(METHOD_BLE,new Connection(METHOD_BLE,"Bluetooth Low Energy"));
    connections.add(METHOD_WIFI,new Connection(METHOD_WIFI,"WiFi"));
    connections.add(METHOD_AJ,new Connection(METHOD_BT,"Audio Jack"));
  }

  public ArrayList<Connection> list() {
    return connections;
  }
}
