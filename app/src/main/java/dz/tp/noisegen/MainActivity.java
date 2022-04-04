package dz.tp.noisegen;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import android.Manifest;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.MediaScannerConnection;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

import android.os.Bundle;
import android.widget.TextView;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.nio.charset.StandardCharsets;

import dz.tp.noisegen.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'noisegen' library on application startup.
    static {
        System.loadLibrary("noisegen");
    }

    private ActivityResultLauncher<String> requestPermissionLauncher =
            registerForActivityResult(new ActivityResultContracts.RequestPermission(), isGranted -> {
            });


    private ActivityMainBinding binding;
    ImageView imgView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        requestPermissionLauncher.launch(
                Manifest.permission.READ_EXTERNAL_STORAGE);
        requestPermissionLauncher.launch(
                Manifest.permission.MANAGE_EXTERNAL_STORAGE);
        requestPermissionLauncher.launch(
                Manifest.permission.WRITE_EXTERNAL_STORAGE);


        imgView = binding.img;


        // Calling C++ function
        byte[] output = generateNoise(32,1,1);

        Bitmap image = BitmapFactory.decodeByteArray( output, 0 ,output.length );

        imgView.setImageBitmap(image);

    }


    public native String stringFromJNI();
    public native byte[] generateNoise(int seed,double frequency, int octaves);

    public void gen(View view) {

        EditText s = findViewById(R.id.seed);
        EditText f = findViewById(R.id.frequency);
        EditText o = findViewById(R.id.octaves);

        byte[] output = generateNoise(
                 Integer.parseInt(s.getText().toString())
                ,Double.parseDouble(f.getText().toString()),
                 Integer.parseInt(o.getText().toString())
        );

        Bitmap image = BitmapFactory.decodeByteArray( output, 0 ,output.length );

        imgView.setImageBitmap(image);
    }
}