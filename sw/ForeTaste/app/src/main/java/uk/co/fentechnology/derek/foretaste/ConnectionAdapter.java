package uk.co.fentechnology.derek.foretaste;

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.simprints.scanner.library.Connection;
import com.simprints.scanner.library.Connector;

/**
 * Created by derek on 21/08/2015.
 */
public class ConnectionAdapter extends BaseAdapter
{
  private Activity activity;
  private Connector connector;
  private LayoutInflater layoutInflater;

  public ConnectionAdapter(Activity activity)
  {
    this.activity = activity;
    connector = Connector.getInstance();
    layoutInflater = LayoutInflater.from(activity);
  }

  @Override
  public int getCount() {
    return connector.size();
  }

  @Override
  public Connection getItem(int position)
  {
    return connector.get(position);
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
      convertView = layoutInflater.inflate(R.layout.viewitem_connection, parent, false);
      // find and save one off items
      viewHolder = new ViewHolder();
      viewHolder.textName = (TextView) convertView.findViewById(R.id.textName);
      viewHolder.textStatus = (TextView) convertView.findViewById(R.id.textStatus);
      viewHolder.button = (Button) convertView.findViewById(R.id.buttonInit);
      viewHolder.button.setOnClickListener(new View.OnClickListener() {
        @Override
        public void onClick(View v)
        {
          Connection connection = connector.get(position);
          if (!connection.init())
          {
            Toast.makeText(v.getContext(),connection.getErrorMessage(),Toast.LENGTH_LONG).show();
          }
          notifyDataSetChanged();
        }
      });
      convertView.setTag(viewHolder);
    }
    else
    {
      // retrieve
      viewHolder = (ViewHolder)convertView.getTag();
    }

    // set texts
    Connection connection = getItem(position);
    viewHolder.textName.setText(connection.name());
    viewHolder.textStatus.setText(connection.isSetup() ? "Setup" : "Not ready");

    return convertView;
  }

  class ViewHolder {
    TextView textName;
    TextView textStatus;
    Button button;
  }
}
