package com.simprints.scanner.library;

import android.util.Log;

import java.util.ArrayList;

/**
 * Created by derek on 18/08/2015.
 */
public class Connector
{
  private ScannerLib scannerLib = ScannerLib.getInstance();

  public Connector() {
  }

  public boolean isSetup(int method) {
    return false; // connections.get(method).isSetup();
  }

  public Scanner getScanner(int method) {
    switch (method)
    {
      case ScannerLib.METHOD_USB:
        Log.v("getScanner","USB");
        break;
      case ScannerLib.METHOD_BT:
        Log.v("getScanner","BT");
        break;
      case ScannerLib.METHOD_BLE:
        Log.v("getScanner","BLE");
        break;
      case ScannerLib.METHOD_WIFI:
        Log.v("getScanner","WIFI");
        break;
      case ScannerLib.METHOD_AJ:
        Log.v("getScanner","AJ");
        break;
      default:
        Log.v("getScanner","unknown method");
        break;
    }

    return new Scanner(method);
  }
}
