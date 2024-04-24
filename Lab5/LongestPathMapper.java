// LongestPathMapper.java
import java.io.IOException;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.*;

public class LongestPathMapper extends Mapper<LongWritable, Text, IntWritable, Text> {
    @Override
    public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
        String path = value.toString();
        int length = path.length();
        context.write(new IntWritable(length), new Text(path));
    }
}