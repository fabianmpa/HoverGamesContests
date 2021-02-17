package com.example.hovergamesapplicationmap;
import android.graphics.Color;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.example.hovergamesapplicationmap.R;
import com.mapbox.geojson.Feature;
import com.mapbox.geojson.LineString;
import com.mapbox.geojson.FeatureCollection;
import com.mapbox.geojson.Point;
import com.mapbox.mapboxsdk.Mapbox;
import com.mapbox.mapboxsdk.maps.MapView;
import com.mapbox.mapboxsdk.maps.MapboxMap;
import com.mapbox.mapboxsdk.maps.OnMapReadyCallback;
import com.mapbox.mapboxsdk.maps.Style;
import com.mapbox.mapboxsdk.style.layers.SymbolLayer;
import com.mapbox.mapboxsdk.style.sources.GeoJsonSource;
import java.util.ArrayList;
import java.util.List;
import com.mapbox.mapboxsdk.style.layers.LineLayer;
import com.mapbox.mapboxsdk.style.layers.Property;
import com.mapbox.mapboxsdk.style.layers.PropertyFactory;
import static com.mapbox.mapboxsdk.style.layers.PropertyFactory.iconAllowOverlap;
import static com.mapbox.mapboxsdk.style.layers.PropertyFactory.iconIgnorePlacement;
import static com.mapbox.mapboxsdk.style.layers.PropertyFactory.iconImage;

/**
 * Display {@link SymbolLayer} icons on the map.
 */
public class MainActivity extends AppCompatActivity implements
        OnMapReadyCallback {

    private static final String SOURCE_ID = "SOURCE_ID";
    private static final String ICON_ID = "ICON_ID";
    private static final String LAYER_ID = "LAYER_ID";
    private MapView mapView;
    private List<Point> routeCoordinates;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

// Mapbox access token is configured here. This needs to be called either in your application
// object or in the same activity which contains the mapview.
        Mapbox.getInstance(this, getString(R.string.mapbox_access_token));

// This contains the MapView in XML and needs to be called after the access token is configured.
        setContentView(R.layout.activity_main);
        mapView = findViewById(R.id.mapView);
        mapView.onCreate(savedInstanceState);
        mapView.getMapAsync(new OnMapReadyCallback() {
            @Override
            public void onMapReady(@NonNull final MapboxMap mapboxMap) {

                mapboxMap.setStyle(Style.OUTDOORS, new Style.OnStyleLoaded() {
                    @Override
                    public void onStyleLoaded(@NonNull Style style) {

                        initRouteCoordinates();

// Create the LineString from the list of coordinates and then make a GeoJSON
// FeatureCollection so we can add the line to our map as a layer.
                        style.addSource(new GeoJsonSource("line-source",
                                FeatureCollection.fromFeatures(new Feature[] {Feature.fromGeometry(
                                        LineString.fromLngLats(routeCoordinates)
                                )})));

// The layer properties for our line. This is where we make the line dotted, set the
// color, etc.
                        style.addLayer(new LineLayer("linelayer", "line-source").withProperties(
                                PropertyFactory.lineDasharray(new Float[] {0.01f, 2f}),
                                PropertyFactory.lineCap(Property.LINE_CAP_ROUND),
                                PropertyFactory.lineJoin(Property.LINE_JOIN_ROUND),
                                PropertyFactory.lineWidth(5f),
                                PropertyFactory.lineColor(Color.parseColor("#e55e5e"))
                        ));
                    }
                });
            }
        });
    }

    private void initRouteCoordinates() {
// Create a list to store our line coordinates.
        routeCoordinates = new ArrayList<>();
        routeCoordinates.add(Point.fromLngLat(-100.475509, 20.739552));
        routeCoordinates.add(Point.fromLngLat(-100.475287, 20.739579));
        routeCoordinates.add(Point.fromLngLat(-100.474984, 20.739610));
        routeCoordinates.add(Point.fromLngLat(-100.474622, 20.739589));
        routeCoordinates.add(Point.fromLngLat(-100.474270, 20.739487));
        routeCoordinates.add(Point.fromLngLat(-100.473903, 20.739231));
        routeCoordinates.add(Point.fromLngLat(-100.473895, 20.738890));
        routeCoordinates.add(Point.fromLngLat(-100.473758, 20.738674));
        routeCoordinates.add(Point.fromLngLat(-100.473557, 20.738556));
        routeCoordinates.add(Point.fromLngLat(-100.473487, 20.738388));
        routeCoordinates.add(Point.fromLngLat(-100.473490, 20.738092));
        routeCoordinates.add(Point.fromLngLat(-100.473592, 20.737867));
        routeCoordinates.add(Point.fromLngLat(-100.473688, 20.737743));
    }

    @Override
    public void onMapReady(@NonNull final MapboxMap mapboxMap) {

        List<Feature> symbolLayerIconFeatureList = new ArrayList<>();
        symbolLayerIconFeatureList.add(Feature.fromGeometry(
                Point.fromLngLat(-100.4781314, 20.7399188)));

        mapboxMap.setStyle(new Style.Builder().fromUri("mapbox://styles/mapbox/cjf4m44iw0uza2spb3q0a7s41")

// Add the SymbolLayer icon image to the map style
                .withImage(ICON_ID, BitmapFactory.decodeResource(
                        MainActivity.this.getResources(), R.drawable.mapbox_marker_icon_default))

// Adding a GeoJson source for the SymbolLayer icons.
                .withSource(new GeoJsonSource(SOURCE_ID,
                        FeatureCollection.fromFeatures(symbolLayerIconFeatureList)))

// Adding the actual SymbolLayer to the map style. An offset is added that the bottom of the red
// marker icon gets fixed to the coordinate, rather than the middle of the icon being fixed to
// the coordinate point. This is offset is not always needed and is dependent on the image
// that you use for the SymbolLayer icon.
                .withLayer(new SymbolLayer(LAYER_ID, SOURCE_ID)
                        .withProperties(
                                iconImage(ICON_ID),
                                iconAllowOverlap(true),
                                iconIgnorePlacement(true)
                        )
                ), new Style.OnStyleLoaded() {
            @Override
            public void onStyleLoaded(@NonNull Style style) {

// Map is set up and the style has loaded. Now you can add additional data or make other map adjustments.


            }
        });
    }

    @Override
    public void onResume() {
        super.onResume();
        mapView.onResume();
    }

    @Override
    protected void onStart() {
        super.onStart();
        mapView.onStart();
    }

    @Override
    protected void onStop() {
        super.onStop();
        mapView.onStop();
    }

    @Override
    public void onPause() {
        super.onPause();
        mapView.onPause();
    }

    @Override
    public void onLowMemory() {
        super.onLowMemory();
        mapView.onLowMemory();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mapView.onDestroy();
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        mapView.onSaveInstanceState(outState);
    }
}
