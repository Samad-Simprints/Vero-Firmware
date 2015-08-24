package uk.co.fentechnology.derek.foretaste;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.simprints.scanner.library.Connection;
import com.simprints.scanner.library.Connector;
import com.simprints.scanner.library.ScannerLib;

import java.util.ArrayList;

/**
 * Created by derek on 21/08/2015.
 */
public class ConnectionAdapter extends BaseAdapter
{
  private ArrayList<Connection> connections;
  LayoutInflater layoutInflater;

  public ConnectionAdapter(Context context)
  {
    ScannerLib scannerLib = ScannerLib.getInstance();
    connections = scannerLib.list();
    layoutInflater = LayoutInflater.from(context);
  }

  @Override
  public int getCount() {
    return connections.size();
  }

  @Override
  public Connection getItem(int position)
  {
    return connections.get(position);
  }

  @Override
  public long getItemId(int position)
  {
    return position;
  }

  @Override
  public View getView(int position, View convertView, ViewGroup parent)
  {
    if (convertView==null)
    {
      convertView = layoutInflater.inflate(R.layout.viewitem_connection,parent,false);
    }

    TextView textView1 = (TextView)convertView.findViewById(R.id.textView1);
    TextView textView2 = (TextView)convertView.findViewById(R.id.textView2);

    Connection connection = getItem(position);

    textView1.setText(connection.name());
    textView2.setText(connection.isSetup()? "Setup": "Not ready");

    return convertView;
  }
}
