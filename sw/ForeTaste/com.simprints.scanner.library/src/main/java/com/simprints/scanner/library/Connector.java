package com.simprints.scanner.library;

import android.util.Log;

import java.util.ArrayList;

/**
 * Created by derek on 18/08/2015.
 */
public class Connector
{
  private static Connector ourInstance = new Connector();

  // arrayList of Connections
  private static final int CONNECTION_SIZE = 4;
  private ArrayList<Connection> connections = new ArrayList<Connection>(CONNECTION_SIZE);

  // singleton "constructor"
  public static Connector getInstance()
  {
    return ourInstance;
  }

  // private constructor
  private Connector() {
    connections.add(new DemoConnection());
    connections.add(new BtConnection());
    connections.add(new UsbConnection());
    connections.add(new BleConnection());
  }

  // public interface
  public ArrayList<Connection> list() {
    return connections;
  }

  public Connection get(int index) {
    return connections.get(index);
  }

  public void init() {
    for (Connection c: connections)
    {
      c.init();
    }
  }
  
  public Connection autoConnect() {
    // @TODO: returns first connection for now
    Connection auto = connections.get(0);
    auto.init();
    return auto;
  }

  public int size() {
    return CONNECTION_SIZE;
  }
}
