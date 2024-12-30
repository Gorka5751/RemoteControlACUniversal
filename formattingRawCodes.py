def format_ir_signal(raw_signal):
    # Split the signal into lines and remove leading/trailing whitespace
    lines = raw_signal.strip().split("\n")

    # Flatten the signal into a single list of values
    flat_signal = []
    for line in lines:
        # Remove the '+' and '-' signs and split the line into individual numbers
        clean_line = line.replace("+", "").replace("-", "").strip()
        # Handle both space and comma-separated values
        numbers = clean_line.replace(",", " ").split()
        # Add numbers to the flat_signal list
        flat_signal.extend(map(int, numbers))

    # Return the formatted signal as a comma-separated list
    return ", ".join(map(str, flat_signal))

# Input: Raw signal
raw_signal = """
 + 500,-1600 + 550,- 550 + 500,-1600 + 550,-1600
 + 500,- 550 + 500,- 550 + 500,-1650 + 500,- 550
 + 550,- 500 + 500,-1650 + 500,- 550 + 500,- 550
 + 500,-1650 + 500,-1600 + 500,- 550 + 550,-1600
 + 500,- 550 + 550,-1600 + 500,- 550 + 550,-1550
 + 550,-1600 + 550,-1600 + 500,-1600 + 550,-1550
 + 550,-1600 + 500,- 550 + 500,-1600 + 550,- 550
 + 500,- 550 + 550,- 550 + 450,- 600 + 500,- 550
 + 500,-1600 + 550,- 550 + 500,-1600 + 550,- 550
 + 450,-1650 + 500,-1650 + 450,- 600 + 500,- 550
 + 500,- 550 + 500,-1650 + 500,- 550 + 500,-1650
 + 500,- 550 + 500,- 550 + 500,-1650 + 450,-1650
 + 500,-5200 +4300,-4350 + 500,-1650 + 500,- 550
 + 500,-1600 + 550,-1600 + 500,- 550 + 500,- 550
 + 550,-1600 + 500,- 550 + 500,- 550 + 550,-1600
 + 500,- 550 + 500,- 550 + 550,-1600 + 500,-1600
 + 500,- 600 + 500,-1600 + 500,- 550 + 550,-1600
 + 500,- 550 + 500,-1650 + 500,-1600 + 500,-1650
 + 500,-1600 + 500,-1650 + 500,-1600 + 500,- 550
 + 550,-1600 + 500,- 600 + 450,- 550 + 550,- 550
 + 500,- 550 + 500,- 600 + 450,-1650 + 500,- 550
 + 500,-1650 + 450,- 600 + 500,-1600 + 500,-1650
 + 500,- 550 + 500,- 550 + 500,- 600 + 450,-1650
 + 500,- 550 + 500,-1650 + 500,- 550 + 500,- 550
 + 500,-1650 + 500,-1600 + 500
"""

# Process the raw signal
formatted_signal = format_ir_signal(raw_signal)

# Print the formatted signal
print("Formatted Signal:")
print(formatted_signal)
