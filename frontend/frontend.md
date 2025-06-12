# Modularized Frontend Structure

```
frontend/
├── src/
│   ├── components/
│   │   ├── ui/                       # shadcn/ui components
│   │   │   ├── alert.jsx
│   │   │   ├── button.jsx
│   │   │   ├── card.jsx
│   │   │   ├── input.jsx
│   │   │   ├── select.jsx
│   │   │   └── tabs.jsx
│   │   │
│   │   ├── ArithmeticCalculator.jsx  # Arithmetic calculator component
│   │   ├── BenchmarkViewer.jsx       # Benchmark visualization component
│   │   ├── LinearSolver.jsx          # Linear equation solver component
│   │   └── MatrixCalculator.jsx      # Matrix calculator component
│   │
│   ├── hooks/
│   │   └── useApi.js                 # Custom API hook
│   │
│   ├── lib/
│   │   └── utils.js                  # Utility functions
│   │
│   ├── App.jsx                       # Main app component
│   ├── index.jsx                     # Entry point
│   └── index.css                     # Global styles
│
├── index.html                        # HTML entry point
├── package.json                      # Dependencies
├── postcss.config.js                 # PostCSS configuration
├── tailwind.config.js                # Tailwind CSS configuration
└── vite.config.js                    # Vite configuration
```

## Component Organization

The frontend has been modularized into the following structure:

1. **Main Components**:
   - `App.jsx`: The root component that sets up the tab navigation
   - `ArithmeticCalculator.jsx`: Component for basic arithmetic operations
   - `MatrixCalculator.jsx`: Component for matrix multiplication
   - `LinearSolver.jsx`: Component for solving linear equations
   - `BenchmarkViewer.jsx`: Component for visualizing benchmark data

2. **UI Components**:
   - All shadcn/ui components are in the `components/ui` directory
   - These provide consistent styling and functionality

3. **Hooks**:
   - `useApi.js`: Custom hook for API communication

4. **Utilities**:
   - `utils.js`: Utility functions for styling with Tailwind CSS

## Benefits of This Structure

1. **Maintainability**: Each component has a single responsibility
2. **Readability**: Smaller files are easier to understand
3. **Reusability**: Components can be reused across the application
4. **Testability**: Isolated components are easier to test
5. **Collaboration**: Multiple developers can work on different components simultaneously
