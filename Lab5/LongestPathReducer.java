// LongestPathReducer.java
import java.io.IOException;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.*;

public class LongestPathReducer extends Reducer<IntWritable, Text, IntWritable, Text> {
    @Override
    public void reduce(IntWritable key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
        for (Text value : values) {
            context.write(key, value);
        }
    }
}