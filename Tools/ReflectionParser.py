import re
from pathlib import Path

# ==============================================================================
# 1. CONFIGURATION & DIRECTORIES
# ==============================================================================

PROJECT_ROOT = Path(__file__).resolve().parents[1]
SOURCE_DIRS = (PROJECT_ROOT / "include", PROJECT_ROOT / "src")
GENERATED_DIR = PROJECT_ROOT / "build" / "generated"

# ==============================================================================
# 2. TYPE REGISTRY
# ==============================================================================

TYPE_REGISTRY = {
    "float": {
        "enum": "PropertyType::Float",
        "getter": "std::to_string({name})",
        "setter": "{name} = std::stof({val})",
    },
    "double": {
        "enum": "PropertyType::Float",
        "getter": "std::to_string({name})",
        "setter": "{name} = std::stod({val})",
    },
    "int": {
        "enum": "PropertyType::Int",
        "getter": "std::to_string({name})",
        "setter": "{name} = std::stoi({val})",
    },
    "bool": {
        "enum": "PropertyType::Bool",
        "getter": "({name} ? \"true\" : \"false\")",
        "setter": "{name} = ({val} == \"true\" || {val} == \"1\")",
    },
    "string": {
        "enum": "PropertyType::String",
        "getter": "{name}",
        "setter": "{name} = {val}",
    },
    "std::string": {
        "enum": "PropertyType::String",
        "getter": "{name}",
        "setter": "{name} = {val}",
    },
    "Vector2": {
        "enum": "PropertyType::Vector2",
        "getter": "std::to_string({name}.x) + \",\" + std::to_string({name}.y)",
        "setter": "sscanf({val}.c_str(), \"%f,%f\", &{name}.x, &{name}.y)",
    },
    "Vector3": {
        "enum": "PropertyType::Vector3",
        "getter": "std::to_string({name}.x) + \",\" + std::to_string({name}.y) + \",\" + std::to_string({name}.z)",
        "setter": "sscanf({val}.c_str(), \"%f,%f,%f\", &{name}.x, &{name}.y, &{name}.z)",
    },
}

# ==============================================================================
# 3. CODE GENERATION HELPERS
# ==============================================================================

def get_property_expression(property_type: str, property_name: str) -> str:
    clean_type = property_type.strip()
    config = TYPE_REGISTRY.get(clean_type)

    if not config:
        for key, cfg in TYPE_REGISTRY.items():
            if key in clean_type:
                config = cfg
                break

    if not config:
        return (
            f'{{"{property_name}", PropertyType::String, '
            f'[this](){{ return "Unsupported"; }}, '
            f'[this](const std::string&){{}}}}'
        )

    enum_type = config["enum"]
    getter_code = config["getter"].format(name=property_name)
    setter_code = config["setter"].format(name=property_name, val="value")

    return (
        f'{{"{property_name}", {enum_type}, '
        f'[this](){{ return {getter_code}; }}, '
        f'[this](const std::string& value){{ {setter_code}; }}}}'
    )


def generate_macro_block(file_id: str, line_number: int, properties) -> str:
    macro_name = f"{file_id}_{line_number}"
    lines = [
        f"#define {macro_name}() \\",
        "public: \\",
        "    std::vector<PropertyInfo> GetProperties() override { \\",
        "        std::vector<PropertyInfo> properties; \\",
    ]

    for p_type, p_name in properties:
        expr = get_property_expression(p_type, p_name)
        lines.append(f"        properties.push_back({expr}); \\")

    lines.extend([
        "        return properties; \\",
        "    }"
    ])
    return "\n".join(lines)


# ==============================================================================
# 4. PARSING & FILE GENERATION LOGIC
# ==============================================================================

def read_text(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8")
    except UnicodeDecodeError:
        return path.read_text(encoding="latin-1")


def find_class_blocks(content: str):
    class_pattern = re.compile(
        r"\b(class|struct)\s+([A-Za-z_][A-Za-z0-9_]*)[^{};]*\{"
    )
    blocks = []
    for match in class_pattern.finditer(content):
        depth = 1
        end_index = -1
        for index in range(match.end(), len(content)):
            if content[index] == "{":
                depth += 1
            elif content[index] == "}":
                depth -= 1
                if depth == 0:
                    end_index = index
                    break
        if end_index != -1:
            blocks.append((match.start(), match.end(), end_index, match.group(2)))
    return blocks


def find_properties(class_body: str):
    property_pattern = re.compile(
        r"SPROPERTY\s*\(\s*[^)]*\s*\)\s*;?\s*"
        r"([A-Za-z_][A-Za-z0-9_:<>\s*&]*)\s+"
        r"([A-Za-z_][A-Za-z0-9_]*)\s*(?:=[^;]+)?;"
    )
    return [
        (p_type.strip(), p_name)
        for p_type, p_name in property_pattern.findall(class_body)
    ]


def inject_include(content: str, generated_filename: str) -> str:
    include_line = f'#include "{generated_filename}"'

    # Remove existing generated include to ensure it gets positioned as the LAST include
    content = re.sub(
        r'^\s*#include\s+["<]' + re.escape(generated_filename) + r'[">]\s*\n?',
        '',
        content,
        flags=re.MULTILINE
    )

    # Insert after the last #include in the file
    includes = list(re.finditer(r'^\s*#include\s+.*$', content, re.MULTILINE))
    if includes:
        last_include_end = includes[-1].end()
        return content[:last_include_end] + "\n" + include_line + content[last_include_end:]

    # Fallback to after #pragma once if no other includes exist
    pragma_match = re.search(r'^\s*#pragma once\s*$', content, re.MULTILINE)
    if pragma_match:
        insertion = pragma_match.end()
        return content[:insertion] + "\n\n" + include_line + content[insertion:]

    return include_line + "\n\n" + content


def process_header(source_path: Path, generated_path: Path):
    content = read_text(source_path)
    class_blocks = find_class_blocks(content)
    
    # Check if there are reflected properties in this file
    has_reflection = any(
        find_properties(content[header_end:end_pos])
        for _, header_end, end_pos, _ in class_blocks
    )

    if not has_reflection:
        return

    # 1. Inject GENERATED_BODY(); into reflected classes if missing
    modified = False
    for start_pos, header_end, end_pos, class_name in reversed(class_blocks):
        class_body = content[header_end:end_pos]
        properties = find_properties(class_body)
        if properties:
            if not re.search(r"\bGENERATED_BODY\s*\(\s*\)\s*;", class_body):
                content = content[:end_pos] + "\n    GENERATED_BODY();\n" + content[end_pos:]
                modified = True

    # 2. Inject generated include as the LAST #include in the header
    content = inject_include(content, f"{source_path.name}.generated.h")
    source_path.write_text(content, encoding="utf-8")

    # Re-read to get exact line numbers after file edits
    content = read_text(source_path)
    class_blocks = find_class_blocks(content)

    # 3. Calculate exact line numbers for GENERATED_BODY()
    reflected_data = []
    for start_pos, header_end, end_pos, class_name in class_blocks:
        class_body = content[header_end:end_pos]
        properties = find_properties(class_body)
        if properties:
            gen_match = re.search(r"\bGENERATED_BODY\s*\(\s*\)\s*;", class_body)
            if gen_match:
                abs_match_index = header_end + gen_match.start()
                line_number = content[:abs_match_index].count('\n') + 1
                reflected_data.append((line_number, properties))

    if not reflected_data:
        return

    rel_path = source_path.relative_to(PROJECT_ROOT)
    file_id = re.sub(r'[^a-zA-Z0-9_]', '_', str(rel_path))

    # 4. Generate .generated.h file
    generated_path.parent.mkdir(parents=True, exist_ok=True)
    generated_content = (
        "#pragma once\n\n"
        "#undef CURRENT_FILE_ID\n"
        f"#define CURRENT_FILE_ID {file_id}\n\n"
        "#include <string>\n"
        "#include <vector>\n"
        "#include <cstdio>\n\n"
    )

    generated_content += "\n\n".join(
        generate_macro_block(file_id, line_num, props) for line_num, props in reflected_data
    )

    generated_path.write_text(generated_content + "\n", encoding="utf-8")
    print(f"[ReflectionParser] Generated {generated_path.relative_to(PROJECT_ROOT)}")


def process_project():
    if GENERATED_DIR.exists():
        for generated_file in GENERATED_DIR.rglob("*.generated.h"):
            generated_file.unlink()

    for source_dir in SOURCE_DIRS:
        if not source_dir.exists():
            continue
        for source_path in (*source_dir.rglob("*.h"), *source_dir.rglob("*.hpp")):
            relative_path = source_path.relative_to(source_dir)
            generated_path = (
                GENERATED_DIR
                / source_dir.name
                / relative_path.parent
                / f"{source_path.name}.generated.h"
            )
            process_header(source_path, generated_path)


if __name__ == "__main__":
    process_project()