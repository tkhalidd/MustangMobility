import requests
import csv
import time
import pandas as pd

# Load the CSV file
file_path = '//Updated_Accessible_Entrances_with_Building_Codes.csv'
df = pd.read_csv(file_path, delimiter=',', skiprows=1)  # Skip the first row if it's an extra header
df.columns = df.columns.str.strip()  # Clean up column names by stripping whitespace

# Define a function to call the Google Maps API with a retry mechanism
def get_walking_distance(lat1, lng1, lat2, lng2, api_key, max_retries=3):
    url = (
        f"https://maps.googleapis.com/maps/api/distancematrix/json"
        f"?origins={lat1},{lng1}&destinations={lat2},{lng2}"
        f"&mode=walking&key={api_key}"
    )
    retries = 0
    while retries < max_retries:
        try:
            response = requests.get(url)
            if response.status_code == 200:
                data = response.json()
                if data["rows"]:
                    distance = data["rows"][0]["elements"][0]["distance"]["value"]  # meters
                    duration = data["rows"][0]["elements"][0]["duration"]["value"]  # seconds
                    return distance, duration
            else:
                print(f"API request failed with status code: {response.status_code}")
            break
        except requests.exceptions.RequestException as e:
            print(f"Connection error: {e}. Retrying ({retries+1}/{max_retries})...")
            retries += 1
            time.sleep(2)  # Wait before retrying
    return None, None

# Define a function to calculate distances with retry and resume capability
def calculate_distances_with_building_codes(input_df, output_file, api_key):
    try:
        # Check if output file exists and load existing rows to resume progress
        existing_rows = set()
        try:
            with open(output_file, 'r') as f:
                reader = csv.reader(f)
                next(reader)  # Skip header
                for row in reader:
                    existing_rows.add((row[0], row[4]))  # Use start and end building codes as unique identifier
        except FileNotFoundError:
            pass  # File doesn't exist yet; will be created below

        # Open output file in append mode to continue writing
        with open(output_file, 'a', newline='') as outfile:
            csv_writer = csv.writer(outfile)
            
            # Write header only if starting from scratch
            if not existing_rows:
                csv_writer.writerow([
                    "Start_Building_Code", "Start_Lat", "Start_Lng", "Start_Name",
                    "End_Building_Code", "End_Lat", "End_Lng", "End_Name",
                    "Distance_m", "Duration_s", "Building_Pair"
                ])
            
            num_locations = len(input_df)
            print(f"Starting distance calculation for {num_locations} locations...")

            # Iterate through all pairs of locations
            for i in range(num_locations):
                row1 = input_df.iloc[i]
                lat1, lng1 = row1['latitude'], row1['longitude']
                building_code_1 = row1['Building_Code']
                start_name = row1['name']

                for j in range(i + 1, num_locations):
                    row2 = input_df.iloc[j]
                    lat2, lng2 = row2['latitude'], row2['longitude']
                    building_code_2 = row2['Building_Code']
                    end_name = row2['name']

                    # Check if this pair has already been processed
                    if (building_code_1, building_code_2) in existing_rows:
                        continue  # Skip this pair

                    # Generate the building pair string
                    building_pair = f"{building_code_1},{building_code_2}"

                    # Get the walking distance and duration with retry
                    distance, duration = get_walking_distance(lat1, lng1, lat2, lng2, api_key)
                    if distance is not None and duration is not None:
                        csv_writer.writerow([
                            building_code_1, lat1, lng1, start_name,  # Start details
                            building_code_2, lat2, lng2, end_name,    # End details
                            distance, duration, building_pair
                        ])
                        print(f"Wrote to CSV: {start_name} ({building_code_1}) -> {end_name} ({building_code_2}) Distance: {distance} Duration: {duration}")
                    else:
                        print(f"Failed to get distance for {start_name} ({building_code_1}) to {end_name} ({building_code_2})")

                    # Add delay to avoid exceeding API rate limits
                    time.sleep(1)

        print(f"File created successfully at {output_file}")

    except Exception as e:
        print(f"An error occurred while writing to the file: {e}")

api_key = ""  
output_file_path = '//precalculated_distances_with_building_codes.csv'
calculate_distances_with_building_codes(df, output_file_path, api_key)
