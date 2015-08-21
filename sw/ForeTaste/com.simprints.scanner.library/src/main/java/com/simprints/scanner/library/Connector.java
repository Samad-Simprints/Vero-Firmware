package com.simprints.scanner.library;

import android.util.Log;

import java.util.ArrayList;

/**
 * Created by derek on 18/08/2015.
 */
public class Connector
{
  public static final int METHOD_USB = 0;
  public static final int METHOD_BT = 1;
  public static final int METHOD_BLE = 2;
  public static final int METHOD_WIFI = 3;
  public static final int METHOD_AJ = 4;

  // only USB and BT are implemented
  public static final int METHOD_COUNT = 2;

  // arrayList of Connections, indexed by method
  private ArrayList<Connection> connections = new ArrayList<Connection>(METHOD_COUNT);

  public Connector() {
    connections.add(METHOD_USB,new Connection(METHOD_USB,"USB"));
    connections.add(METHOD_BT,new Connection(METHOD_BT,"Bluetooth"));
  }

  public ArrayList<Connection> list() {
    return connections;
  }

  public boolean isSetup(int method) {
    return connections.get(method).isSetup();
  }

  public Scanner getScanner(int method) {
    switch (method)
    {
      case METHOD_USB:
        Log.v("getScanner","USB");
        break;
      case METHOD_BT:
        Log.v("getScanner","BT");
        break;
      case METHOD_BLE:
        Log.v("getScanner","BLE");
        break;
      case METHOD_WIFI:
        Log.v("getScanner","WIFI");
        break;
      case METHOD_AJ:
        Log.v("getScanner","AJ");
        break;
      default:
        Log.v("getScanner","unknown method");
        break;
    }

    return new Scanner(method);
  }
}
