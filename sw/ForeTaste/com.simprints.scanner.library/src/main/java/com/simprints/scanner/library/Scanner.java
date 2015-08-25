package com.simprints.scanner.library;

/**
 * Created by derek on 18/08/2015.
 */
public class Scanner
{
  private Connection connection;

  public Scanner(Connection connection) {
    this.connection = connection;
  }

  public void getTemplate(Template template) {
    template = new Template();
  }

}
