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

}
