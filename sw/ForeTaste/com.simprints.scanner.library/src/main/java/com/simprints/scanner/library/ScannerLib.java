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

  // only DEMO is implemented
  public static final int CONNECTION_COUNT = 1;

  // arrayList of Connections, indexed by method
  private ArrayList<Connection> connections = new ArrayList<Connection>(CONNECTION_COUNT);

  // singleton "constructor"
  public static ScannerLib getInstance()
  {
    return ourInstance;
  }

  // private constructor
  private ScannerLib()
  {
    connections.add(new DemoConnection());
    /*
    connections.add(new UsbConnection("USB"));
    connections.add(new BtConnection("Bluetooth Classic"));
    connections.add(new BleConnection("Bluetooth Low Energy"));
    connections.add(new WifiConnection("WiFi"));
    connections.add(new AjConnection("Audio Jack"));
    */
  }

  // public interface
  public ArrayList<Connection> list() {
    return connections;
  }

  public Connection get(int index) {
    return connections.get(index);
  }
}
