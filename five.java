import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
public class five {

    public class Almanac {

        private static List<Long[]> seeds = new ArrayList<Long[]>();
        private static Map<Long, Long[]> seed_to_soil = new HashMap<Long, Long[]>();
        private static Map<Long, Long[]> soil_to_fertilizer = new HashMap<Long, Long[]>();
        private static Map<Long, Long[]> fertilizer_to_water = new HashMap<Long, Long[]>();
        private static Map<Long, Long[]> water_to_light = new HashMap<Long, Long[]>();
        private static Map<Long, Long[]> light_to_temperature = new HashMap<Long, Long[]>();
        private static Map<Long, Long[]> temperature_to_humidity = new HashMap<Long, Long[]>();
        private static Map<Long, Long[]> humidity_to_location = new HashMap<Long, Long[]>();

        private static long get_value(long source, Map<Long, Long[]> map){
            // given a map of seed : soil, range, and a seed to find
            // returns the value of the soil
            if (map.containsKey(source)){
                // Debugging: System.out.printf("key in map with value: %d\n",map.get(source)[0] );
                return map.get(source)[0];
            }
            // else, either the value is in a range somewhere
            for (var entry : map.entrySet()) {
                var val = entry.getValue();
                var dest = val[0];
                var range = val[1];
                var src = entry.getKey();

                if (source <= src + range && source > src){
                    var diff = dest - src;
                    // Debugging: System.out.printf("key in range in a map, with src %d dest %d and range %d : %d\n", src, dest, range, source + diff);
                    return source + diff;
                }
            }
            // or the value of the key == value
            // Debugging: System.out.printf("key absent from map: %d\n", source );

            return source;
        }

        public static long calculate_solution(){
            long solution = Long.MAX_VALUE;
            for (var seed_range: Almanac.seeds){
                var first_seed = seed_range[0];
                var range = seed_range[1];
                for (long i = first_seed; i < first_seed + range; i ++){
                    // TODO: can potentially optimise this by not doing the calculations for every seed
                    // or reversing the maps, so that instead of going through every seed
                    // (which was fine in part one, when there were fewer seeds than any other vals)
                    // we can sort location map and for smallest key,
                    // find the seed and if it is in the initial input.
                    // That's more of a rewrite though, so just using brute-force for now.
                    var val = get_value(i, Almanac.seed_to_soil);
                    val = get_value(val, Almanac.soil_to_fertilizer);
                    val = get_value(val, Almanac.fertilizer_to_water);
                    val = get_value(val, Almanac.water_to_light);
                    val = get_value(val, Almanac.light_to_temperature);
                    val = get_value(val, Almanac.temperature_to_humidity);
                    val = get_value(val, Almanac.humidity_to_location);

                    solution = Math.min(solution, val);
                }

            }
            return solution;
        }

        private static Map<Long, Long[]> parse_entries(List<String> chunk){
            var map = new HashMap<Long, Long[]>();

            for (int i = 1; i < chunk.size(); i ++){
                var vals = chunk.get(i).split(" ");
                var dest = Long.valueOf(vals[0]);
                var source = Long.valueOf(vals[1]);
                var range = Long.valueOf(vals[2]);
                map.put(source, new Long[] { dest, range });
            }
            return map;
        }

        private static void parse_chunk(List<String> chunk){
            var name = chunk.get(0);
            if (chunk.size() == 1 && name.startsWith("seeds:")){
                String[] _seeds = name.split(" ");
                for (int i = 1; i < _seeds.length; i+=2){
                    seeds.add(new Long[] {Long.valueOf(_seeds[i]), Long.valueOf(_seeds[i+1])});
                }
            } else {
                if (name.equals("seed-to-soil map:")){
                    seed_to_soil = parse_entries(chunk);
                } else if (name.equals("soil-to-fertilizer map:")){
                    soil_to_fertilizer = parse_entries(chunk);
                } else if (name.equals("fertilizer-to-water map:")){
                    fertilizer_to_water = parse_entries(chunk);
                } else if (name.equals("water-to-light map:")){
                    water_to_light = parse_entries(chunk);
                } else if (name.equals("light-to-temperature map:")){
                    light_to_temperature = parse_entries(chunk);
                } else if (name.equals("temperature-to-humidity map:")){
                    temperature_to_humidity = parse_entries(chunk);
                } else if (name.equals("humidity-to-location map:")){
                    humidity_to_location = parse_entries(chunk);
                }

            }
        }

        public static void parse_file(String input_file){
            // File reading code copied from Stack Overflow:
            // https://stackoverflow.com/questions/45826412/how-to-parse-a-simple-text-file-in-java
            FileInputStream stream = null;
            try {
                stream = new FileInputStream(input_file);
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
            BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
            String strLine;
            List<String> current = new ArrayList<String>();
            try {
                while ((strLine = reader.readLine()) != null) {
                    if (strLine.equals("")){
                        parse_chunk(current);
                        current.clear();
                    } else{
                        current.add(strLine);
                    }
                }
                parse_chunk(current);
            } catch (IOException e) {
                e.printStackTrace();
            }
            try {
                reader.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return;
        }
    };


    public static void main(String args[]) {

        Almanac.parse_file(args[0]);
        long solution = Almanac.calculate_solution();
        System.out.printf("Solution: %d\n", solution);
    }

}
