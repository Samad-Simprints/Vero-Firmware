package uk.co.fentechnology.derek.foretaste;

import android.app.Activity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.simprints.scanner.library.Connection;
import com.simprints.scanner.library.Connector;
import com.simprints.scanner.library.Scanner;

/**
 * Created by derek on 27/08/2015.
 */
public class SelectionAdapter extends BaseAdapter
{
  private Connection connection;
  private LayoutInflater layoutInflater;

  public SelectionAdapter(Activity activity,int connectIndex) {
    Connector connector = Connector.getInstance();
    connection = connector.get(connectIndex);
    layoutInflater = LayoutInflater.from(activity);
  }

  @Override
  public int getCount()
  {
    Log.i("SelectionAdapter", "getCount: " + connection.deviceList().length);
    return connection.deviceList().length;
  }

  @Override
  public Object getItem(int position)
  {
    return null;
  }

  @Override
  public long getItemId(int position)
  {
    return position;
  }

  @Override
  public View getView(final int position, View convertView, ViewGroup parent)
  {
    ViewHolder viewHolder;

    if (convertView == null)
    {
      convertView = layoutInflater.inflate(R.layout.viewitem_selection, parent, false);
      // find and save one off items
      viewHolder = new ViewHolder();
      viewHolder.textName = (TextView) convertView.findViewById(R.id.textScannerName);
      viewHolder.textDetail = (TextView) convertView.findViewById(R.id.textScannerDetail);
      convertView.setTag(viewHolder);
    }
    else
    {
      // retrieve
      viewHolder = (ViewHolder)convertView.getTag();
    }

    // set texts
    viewHolder.textName.setText(connection.deviceName(position));
    viewHolder.textDetail.setText(connection.deviceDetail(position));

    return convertView;
  }

  class ViewHolder {
    TextView textName;
    TextView textDetail;
  }
}
