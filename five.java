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

        private static List<Long> seeds = new ArrayList<Long>();
        private static Map<Long, Long> seed_to_soil = new HashMap<Long, Long>();
        private static Map<Long, Long> soil_to_fertilizer = new HashMap<Long, Long>();
        private static Map<Long, Long> fertilizer_to_water = new HashMap<Long, Long>();
        private static Map<Long, Long> water_to_light = new HashMap<Long, Long>();
        private static Map<Long, Long> light_to_temperature = new HashMap<Long, Long>();
        private static Map<Long, Long> temperature_to_humidity = new HashMap<Long, Long>();
        private static Map<Long, Long> humidity_to_location = new HashMap<Long, Long>();

        public static long calculate_solution(){
            long solution = Long.MAX_VALUE;
            for (int i = 0; i < Almanac.seeds.size(); i ++){
                long val = Almanac.seeds.get(i);
                val = Almanac.seed_to_soil.getOrDefault(val, val);
                val = Almanac.soil_to_fertilizer.getOrDefault(val, val);
                val = Almanac.fertilizer_to_water.getOrDefault(val, val);
                val = Almanac.water_to_light.getOrDefault(val, val);
                val = Almanac.light_to_temperature.getOrDefault(val, val);
                val = Almanac.temperature_to_humidity.getOrDefault(val, val);
                val = Almanac.humidity_to_location.getOrDefault(val, val);
                solution = Math.min(solution, val);
            }
            return solution;
        }

        public static Map<Long, Long> parse_entries(List<String> chunk){
            Map<Long, Long> map = new HashMap<Long, Long>();

            for (int i = 1; i < chunk.size(); i ++){
                String[] vals = chunk.get(i).split(" ");
                long dest = Long.valueOf(vals[0]);
                long source = Long.valueOf(vals[1]);
                long range = Long.valueOf(vals[2]);

                for (int j = 0; j < range; j ++){
                    map.put(source + j, dest + j);
                }
            }
            return map;
        }

        public static void parse_chunk(List<String> chunk){
            String name = chunk.get(0);
            if (chunk.size() == 1 && name.startsWith("seeds:")){
                String[] _seeds = name.split(" ");
                for (int i = 1; i < _seeds.length; i++){
                    seeds.add(Long.valueOf(_seeds[i]));
                }
            } else {
                if (name.equals("seed-to-soil map:")){
                    seed_to_soil = parse_entries(chunk);
                }
                else if (name.equals("soil-to-fertilizer map:")){
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
                current.clear();
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
