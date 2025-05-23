import os

def print_folder_structure(root_path, indent=""):
    for root, dirs, files in os.walk(root_path):
        level = root.replace(root_path, "").count(os.sep)
        indent_space = " " * 4 * level
        print(f"{indent_space}|-- {os.path.basename(root)}/")
        sub_indent = " " * 4 * (level + 1)
        for file in files:
            print(f"{sub_indent}|-- {file}")
        # Prevent walking into subdirs again manually
        dirs[:] = []


def print_tree(dir_path, prefix=""):
    # Get list of all entries in directory
    entries = sorted(os.listdir(dir_path))
    entries_count = len(entries)
    
    for index, entry in enumerate(entries):
        path = os.path.join(dir_path, entry)
        is_last = index == entries_count - 1
        connector = "└── " if is_last else "├── "
        print(prefix + connector + entry)
        
        if os.path.isdir(path):
            extension = "    " if is_last else "│   "
            print_tree(path, prefix + extension)

# Example usage
if __name__ == "__main__":
    root_dir = "C:/Users/punit/Desktop/VNIT/Sem3/DessertationProject/VMAI/VedicMathEngine"  # Replace with your target folder
    print(root_dir)
    print_tree(root_dir)

