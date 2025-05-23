"""
vedicmath_py.py - Python binding for VedicMath-AI Library
"""
import ctypes
import os
import platform
import numpy as np
import pandas as pd
from typing import Union, List, Tuple, Optional
from dataclasses import dataclass
from enum import Enum

class VedicNumberType(Enum):
    INT32 = 0
    INT64 = 1
    FLOAT = 2
    DOUBLE = 3
    INVALID = 4

class VedicOperation(Enum):
    ADD = 0
    SUBTRACT = 1
    MULTIPLY = 2
    DIVIDE = 3
    MODULO = 4
    POWER = 5
    INVALID = 6

@dataclass
class BenchmarkResult:
    """Performance benchmark result"""
    operation: str
    implementation: str
    operand_a: Union[int, float]
    operand_b: Union[int, float]
    execution_time_ms: float
    sutra_used: str
    confidence_score: int

class VedicMathLibrary:
    """Python wrapper for VedicMath-AI C library"""
    
    def __init__(self, lib_path: Optional[str] = None):
        """Initialize the library wrapper"""
        if lib_path is None:
            lib_path = self._find_library()
        
        self.lib = ctypes.CDLL(lib_path)
        self._setup_function_signatures()
        self._dataset = []
    
    def _find_library(self) -> str:
        """Find the VedicMath library file"""
        system = platform.system().lower()
        
        if system == "windows":
            lib_name = "vedicmath.dll"
        elif system == "darwin":  # macOS
            lib_name = "libvedicmath.dylib"
        else:  # Linux and others
            lib_name = "libvedicmath.so"
        
        # Search in common locations
        search_paths = [
            ".",
            "./build",
            "../build",
            "./lib",
            "../lib"
        ]
        
        for path in search_paths:
            full_path = os.path.join(path, lib_name)
            if os.path.exists(full_path):
                return full_path
        
        raise FileNotFoundError(f"Could not find {lib_name} in search paths")
    
    def _setup_function_signatures(self):
        """Setup C function signatures for proper calling"""
        
        # Basic multiplication functions
        self.lib.vedic_multiply.argtypes = [ctypes.c_long, ctypes.c_long]
        self.lib.vedic_multiply.restype = ctypes.c_long
        
        self.lib.vedic_square.argtypes = [ctypes.c_long]
        self.lib.vedic_square.restype = ctypes.c_long
        
        self.lib.vedic_divide.argtypes = [ctypes.c_long, ctypes.c_long, ctypes.POINTER(ctypes.c_long)]
        self.lib.vedic_divide.restype = ctypes.c_long
        
        # Specific sutras
        self.lib.ekadhikena_purvena.argtypes = [ctypes.c_long]
        self.lib.ekadhikena_purvena.restype = ctypes.c_long
        
        self.lib.nikhilam_mul.argtypes = [ctypes.c_long, ctypes.c_long]
        self.lib.nikhilam_mul.restype = ctypes.c_long
        
        self.lib.urdhva_mult.argtypes = [ctypes.c_long, ctypes.c_long]
        self.lib.urdhva_mult.restype = ctypes.c_long
        
        # Enhanced dispatcher (if available)
        try:
            self.lib.vedic_multiply_enhanced.argtypes = [ctypes.c_long, ctypes.c_long]
            self.lib.vedic_multiply_enhanced.restype = ctypes.c_long
            self.enhanced_available = True
        except AttributeError:
            self.enhanced_available = False
        
        # Dataset functions (if available)
        try:
            self.lib.init_dataset_collection.argtypes = [ctypes.c_size_t]
            self.lib.init_dataset_collection.restype = None
            
            self.lib.generate_test_patterns.argtypes = [ctypes.c_size_t]
            self.lib.generate_test_patterns.restype = None
            
            self.lib.export_dataset_csv.argtypes = [ctypes.c_char_p]
            self.lib.export_dataset_csv.restype = None
            
            self.lib.cleanup_dataset.argtypes = []
            self.lib.cleanup_dataset.restype = None
            
            self.dataset_available = True
        except AttributeError:
            self.dataset_available = False
    
    def multiply(self, a: Union[int, float], b: Union[int, float], 
                enhanced: bool = False) -> Union[int, float]:
        """Multiply two numbers using Vedic mathematics"""
        if enhanced and self.enhanced_available:
            return self.lib.vedic_multiply_enhanced(int(a), int(b))
        else:
            return self.lib.vedic_multiply(int(a), int(b))
    
    def square(self, n: Union[int, float]) -> Union[int, float]:
        """Square a number using Vedic mathematics"""
        return self.lib.vedic_square(int(n))
    
    def divide(self, dividend: Union[int, float], 
              divisor: Union[int, float]) -> Tuple[int, int]:
        """Divide using Vedic mathematics, returns (quotient, remainder)"""
        remainder = ctypes.c_long()
        quotient = self.lib.vedic_divide(int(dividend), int(divisor), 
                                       ctypes.byref(remainder))
        return quotient, remainder.value
    
    def ekadhikena_purvena(self, n: Union[int, float]) -> Union[int, float]:
        """Square numbers ending in 5 using Ekadhikena Purvena"""
        return self.lib.ekadhikena_purvena(int(n))
    
    def nikhilam(self, a: Union[int, float], b: Union[int, float]) -> Union[int, float]:
        """Multiply numbers near a base using Nikhilam"""
        return self.lib.nikhilam_mul(int(a), int(b))
    
    def urdhva_tiryagbhyam(self, a: Union[int, float], 
                          b: Union[int, float]) -> Union[int, float]:
        """General multiplication using Urdhva-Tiryagbhyam"""
        return self.lib.urdhva_mult(int(a), int(b))
    
    def benchmark_multiplication(self, test_cases: List[Tuple[int, int]], 
                               methods: List[str] = None) -> pd.DataFrame:
        """Benchmark different multiplication methods"""
        import time
        
        if methods is None:
            methods = ['standard', 'vedic', 'ekadhikena', 'nikhilam', 'urdhva']
        
        results = []
        
        for a, b in test_cases:
            for method in methods:
                # Time the operation
                start_time = time.perf_counter()
                
                if method == 'standard':
                    result = a * b
                    sutra = 'Standard'
                elif method == 'vedic':
                    result = self.multiply(a, b)
                    sutra = 'Auto-selected'
                elif method == 'ekadhikena' and a == b and a % 10 == 5:
                    result = self.ekadhikena_purvena(a)
                    sutra = 'Ekadhikena Purvena'
                elif method == 'nikhilam':
                    result = self.nikhilam(a, b)
                    sutra = 'Nikhilam'
                elif method == 'urdhva':
                    result = self.urdhva_tiryagbhyam(a, b)
                    sutra = 'Urdhva-Tiryagbhyam'
                else:
                    continue  # Skip invalid methods
                
                end_time = time.perf_counter()
                execution_time = (end_time - start_time) * 1000  # Convert to ms
                
                results.append({
                    'operand_a': a,
                    'operand_b': b,
                    'method': method,
                    'sutra_used': sutra,
                    'result': result,
                    'execution_time_ms': execution_time,
                    'correct': result == a * b
                })
        
        return pd.DataFrame(results)
    
    def generate_dataset(self, pattern_count: int = 1000, 
                        export_csv: bool = True) -> Optional[pd.DataFrame]:
        """Generate dataset using the enhanced dispatcher"""
        if not self.dataset_available:
            print("Dataset generation not available in this build")
            return None
        
        # Initialize dataset collection
        self.lib.init_dataset_collection(pattern_count * 2)
        
        # Generate test patterns
        print(f"Generating {pattern_count} test patterns...")
        self.lib.generate_test_patterns(pattern_count)
        
        # Export to CSV if requested
        if export_csv:
            csv_filename = "vedic_decisions.csv"
            self.lib.export_dataset_csv(csv_filename.encode('utf-8'))
            
            # Read the CSV back into a DataFrame
            try:
                df = pd.read_csv(csv_filename)
                print(f"Dataset exported to {csv_filename}")
                return df
            except FileNotFoundError:
                print("Failed to read exported CSV")
                return None
        
        return None
    
    def analyze_patterns(self, df: pd.DataFrame) -> dict:
        """Analyze the generated dataset patterns"""
        if df is None or df.empty:
            return {}
        
        analysis = {
            'total_records': len(df),
            'avg_execution_time': df['execution_time_ms'].mean(),
            'sutra_distribution': df['sutra_used'].value_counts().to_dict(),
            'confidence_stats': {
                'mean': df['confidence_score'].mean(),
                'std': df['confidence_score'].std(),
                'min': df['confidence_score'].min(),
                'max': df['confidence_score'].max()
            }
        }
        
        return analysis
    
    def create_test_patterns(self, count: int = 100) -> List[Tuple[int, int]]:
        """Create diverse test patterns for benchmarking"""
        np.random.seed(42)  # For reproducible results
        patterns = []
        
        pattern_types = count // 6
        
        # 1. Numbers ending in 5 (Ekadhikena Purvena)
        for _ in range(pattern_types):
            n = np.random.randint(1, 20) * 10 + 5
            patterns.append((n, n))
        
        # 2. Numbers near 100 (Nikhilam)
        for _ in range(pattern_types):
            a = np.random.randint(85, 116)
            b = np.random.randint(85, 116)
            patterns.append((a, b))
        
        # 3. Numbers near 1000 (Nikhilam)
        for _ in range(pattern_types):
            a = np.random.randint(950, 1051)
            b = np.random.randint(950, 1051)
            patterns.append((a, b))
        
        # 4. Antyayordasake patterns
        for _ in range(pattern_types):
            prefix = np.random.randint(1, 10)
            last_a = np.random.randint(1, 10)
            last_b = 10 - last_a
            a = prefix * 10 + last_a
            b = prefix * 10 + last_b
            patterns.append((a, b))
        
        # 5. Large numbers (Urdhva-Tiryagbhyam)
        for _ in range(pattern_types):
            a = np.random.randint(1000, 10000)
            b = np.random.randint(1000, 10000)
            patterns.append((a, b))
        
        # 6. Random numbers
        remaining = count - len(patterns)
        for _ in range(remaining):
            a = np.random.randint(1, 1000)
            b = np.random.randint(1, 1000)
            patterns.append((a, b))
        
        return patterns
    
    def run_comprehensive_benchmark(self, count: int = 1000) -> dict:
        """Run comprehensive benchmarking and analysis"""
        print(f"Running comprehensive benchmark with {count} test cases...")
        
        # Generate test patterns
        test_cases = self.create_test_patterns(count)
        
        # Benchmark different methods
        benchmark_df = self.benchmark_multiplication(test_cases)
        
        # Generate dataset if available
        dataset_df = None
        if self.dataset_available:
            dataset_df = self.generate_dataset(count)
        
        # Analysis
        analysis = {
            'benchmark_results': benchmark_df,
            'performance_summary': self._analyze_performance(benchmark_df),
            'dataset_analysis': self.analyze_patterns(dataset_df) if dataset_df is not None else None
        }
        
        return analysis
    
    def _analyze_performance(self, df: pd.DataFrame) -> dict:
        """Analyze performance from benchmark results"""
        performance = {}
        
        # Group by method
        grouped = df.groupby('method')
        
        for method, group in grouped:
            performance[method] = {
                'avg_time_ms': group['execution_time_ms'].mean(),
                'std_time_ms': group['execution_time_ms'].std(),
                'accuracy': (group['correct'].sum() / len(group)) * 100,
                'total_operations': len(group)
            }
        
        # Calculate speedups relative to standard
        if 'standard' in performance:
            std_time = performance['standard']['avg_time_ms']
            for method in performance:
                if method != 'standard':
                    speedup = std_time / performance[method]['avg_time_ms']
                    performance[method]['speedup_vs_standard'] = speedup
        
        return performance
    
    def __del__(self):
        """Cleanup resources"""
        if hasattr(self, 'lib') and self.dataset_available:
            try:
                self.lib.cleanup_dataset()
            except:
                pass

# Example usage and demo
def demo_vedic_math():
    """Demonstrate the VedicMath Python binding"""
    try:
        # Initialize the library
        vedic = VedicMathLibrary()
        print("VedicMath-AI Python binding initialized successfully!")
        
        # Basic operations
        print("\n=== Basic Operations ===")
        print(f"vedic_multiply(102, 32) = {vedic.multiply(102, 32)}")
        print(f"vedic_square(25) = {vedic.square(25)}")
        print(f"vedic_divide(1234, 56) = {vedic.divide(1234, 56)}")
        
        # Specific sutras
        print("\n=== Specific Sutras ===")
        print(f"Ekadhikena Purvena (25²) = {vedic.ekadhikena_purvena(25)}")
        print(f"Nikhilam (98 × 97) = {vedic.nikhilam(98, 97)}")
        print(f"Urdhva-Tiryagbhyam (123 × 456) = {vedic.urdhva_tiryagbhyam(123, 456)}")
        
        # Performance comparison
        print("\n=== Performance Comparison ===")
        test_cases = [(25, 25), (98, 97), (123, 456), (102, 32)]
        benchmark_df = vedic.benchmark_multiplication(test_cases)
        
        # Display performance summary
        performance = vedic._analyze_performance(benchmark_df)
        for method, stats in performance.items():
            speedup_str = f", Speedup: {stats.get('speedup_vs_standard', 1.0):.2f}x" if 'speedup_vs_standard' in stats else ""
            print(f"{method}: Avg time: {stats['avg_time_ms']:.6f}ms, Accuracy: {stats['accuracy']:.1f}%{speedup_str}")
        
        # Run comprehensive benchmark
        print("\n=== Comprehensive Benchmark ===")
        results = vedic.run_comprehensive_benchmark(100)
        
        print("Performance Summary:")
        for method, stats in results['performance_summary'].items():
            print(f"  {method}: {stats['avg_time_ms']:.6f}ms avg")
        
        if results['dataset_analysis']:
            print(f"\nDataset Analysis:")
            analysis = results['dataset_analysis']
            print(f"  Total records: {analysis['total_records']}")
            print(f"  Avg execution time: {analysis['avg_execution_time']:.6f}ms")
            print("  Sutra distribution:")
            for sutra, count in analysis['sutra_distribution'].items():
                percentage = (count / analysis['total_records']) * 100
                print(f"    {sutra}: {count} ({percentage:.1f}%)")
        
        return vedic
        
    except Exception as e:
        print(f"Error: {e}")
        print("Make sure the VedicMath library is built and accessible")
        return None

if __name__ == "__main__":
    demo_vedic_math()