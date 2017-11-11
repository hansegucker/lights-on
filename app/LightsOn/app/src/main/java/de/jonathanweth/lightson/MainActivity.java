package de.jonathanweth.lightson;

import android.content.Context;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.telecom.Call;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.flask.colorpicker.ColorPickerView;
import com.flask.colorpicker.OnColorSelectedListener;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.reflect.Array;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;
import java.util.concurrent.Callable;


public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Buttons
        final Button AllLightsOn = (Button) findViewById(R.id.all_lights_on);
        final Button AllLightsOff = (Button) findViewById(R.id.all_lights_off);
        final ColorPickerView ColorPicker = (ColorPickerView) findViewById(R.id.color_picker);

        AllLightsOn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                say(getResources().getString(R.string.all_lights_on_do));

                allOn();
            }
        });

        AllLightsOff.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                say(getResources().getString(R.string.all_lights_off_do));

                allOff();
            }
        });

        ColorPicker.addOnColorSelectedListener(new OnColorSelectedListener() {
            @Override
            public void onColorSelected(int selectedColor) {
                say(Integer.toHexString(selectedColor));
                say(getResources().getString(R.string.color_change));
            }
        });
    }

    protected void allOn() {
        class AllOnTask extends GetUrlContentTask {
            @Override
            protected void onPostExecute(String result) {
                say(result);
            }
        }
        new AllOnTask().execute("http://192.168.4.1/all/on/");
    }

    protected void allOff() {
        class AllOnTask extends GetUrlContentTask {
            @Override
            protected void onPostExecute(String result) {
                say(result);
            }
        }
        new AllOnTask().execute("http://192.168.4.1/all/off/");
    }

    protected void say(CharSequence msg) {
        // Show a toast with msg
        Context context = getApplicationContext();
        int duration = Toast.LENGTH_SHORT;
        Toast toast = Toast.makeText(context, msg, duration);
        toast.show();
    }

    private class GetUrlContentTask extends AsyncTask<String, Integer, String> {
        private String sendRequest(String... urls) {
            try {
                // Create url var
                URL url = new URL(urls[0]);

                // Create connection
                HttpURLConnection connection = (HttpURLConnection) url.openConnection();

                // Set connection settings
                connection.setRequestMethod("GET");
                connection.setDoOutput(true);
                connection.setConnectTimeout(5000);
                connection.setReadTimeout(5000);

                // Connect
                connection.connect();

                // Get data
                BufferedReader rd = new BufferedReader(new InputStreamReader(connection.getInputStream()));

                // Write in string
                String content = "", line;
                while ((line = rd.readLine()) != null) {
                    content += line + "\n";
                }

                // Return string
                return content;
            } catch (Exception e) {
                e.printStackTrace();
            }

            return "";
        }

        protected String doInBackground(String... urls) {
            return sendRequest(urls);
        }

        protected void onProgressUpdate(Integer... progress) {
        }

        protected void onPostExecute(String result) {
            say(result);
        }
    }
}


