package com.simprints.scanner.library;

import android.app.Application;
import android.test.ApplicationTestCase;

import java.util.ArrayList;

/**
 * <a href="http://d.android.com/tools/testing/testing_android.html">Testing Fundamentals</a>
 */
public class ApplicationTest extends ApplicationTestCase<Application>
{
  public ApplicationTest()
  {
    super(Application.class);
  }

  public void testconnector() throws Exception
  {
    Connector connector = Connector.getInstance();
    ArrayList<Connection> listConnections = connector.list();
    if (!(listConnections.size()>0)) { throw new AssertionError(); }
    if (!(listConnections.size()==2)) { throw new AssertionError(); }
  }
}
