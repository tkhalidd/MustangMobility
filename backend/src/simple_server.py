import subprocess
import json
import os
from http.server import HTTPServer, SimpleHTTPRequestHandler

class RequestHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        self.current_dir = os.path.dirname(os.path.abspath(__file__))
        project_root = os.path.dirname(os.path.dirname(self.current_dir))
        
        # Store the paths we'll need
        self.executable_path = os.path.join(project_root, 'build', 'Mustang_Mobility')
        self.data_dir = os.path.join(project_root, 'data')
        
        print(f"Serving files from: {project_root}")
        print(f"Executable path: {self.executable_path}")
        print(f"Data directory: {self.data_dir}")
        
        super().__init__(*args, directory=project_root, **kwargs)

    def do_GET(self):
        print(f"GET request for: {self.path}")
        print(f"Looking in directory: {self.directory}")
        return super().do_GET()

    def do_POST(self):
        print(">>> POST request received")
        print(f">>> Path: {self.path}")
        
        try:
            self.send_response(200)
            self.send_header('Access-Control-Allow-Origin', '*')
            self.send_header('Access-Control-Allow-Methods', 'POST, OPTIONS')
            self.send_header('Access-Control-Allow-Headers', 'Content-Type')
            self.send_header('Content-Type', 'application/json')
            self.end_headers()

            if self.path == '/find_path':
                content_length = int(self.headers['Content-Length'])
                post_data = self.rfile.read(content_length)
                
                try:
                    data = json.loads(post_data.decode('utf-8'))
                    stops = data.get('stops', [])
                    print(f">>> Parsed stops: {stops}")

                    # Use the full path to the executable and set working directory
                    command = [self.executable_path] + stops
                    print(f">>> Executing command: {command}")
                    
                    process = subprocess.Popen(
                        command,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE,
                        cwd=self.data_dir
                    )
                    output, error = process.communicate()
                    
                    if process.returncode == 0:
                        raw_output = output.decode('utf-8').strip()
                        lines = raw_output.split('\n')
                        response = {
                            'success': True,
                            'path': lines[0].replace('Complete Path Nodes: ', '').strip(),
                            'distance': lines[1].replace('Total Distance: ', '').strip(),
                            'duration': lines[2].replace('Total Duration: ', '').strip()
                        }
                    else:
                        response = {
                            'success': False,
                            'error': error.decode('utf-8').strip()
                        }

                    self.wfile.write(json.dumps(response).encode('utf-8'))
                except Exception as e:
                    self.wfile.write(json.dumps({
                        'success': False, 
                        'error': str(e)
                    }).encode('utf-8'))
            else:
                self.wfile.write(json.dumps({
                    'success': False,
                    'error': 'Invalid endpoint'
                }).encode('utf-8'))
        except Exception as e:
            try:
                self.send_response(500)
                self.send_header('Content-Type', 'application/json')
                self.end_headers()
                self.wfile.write(json.dumps({
                    'success': False,
                    'error': f'Server error: {str(e)}'
                }).encode('utf-8'))
            except:
                print(">>> Could not send error response")

    def do_OPTIONS(self):
        print(">>> OPTIONS request received")
        self.send_response(200)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()

# Start the server
if __name__ == "__main__":
    httpd = HTTPServer(('localhost', 8000), RequestHandler)
    print("Server running on http://localhost:8000")
    httpd.serve_forever()
