from reportlab.lib import pagesizes
from reportlab.lib.utils import ImageReader
from reportlab.pdfgen import canvas
import os
from PIL import Image
import argparse

parser = argparse.ArgumentParser()

parser.add_argument("--pdf_file")
parser.add_argument("--image_dir")
parser.add_argument("--text_dir")

args = parser.parse_args()


def create_pdf(image_dir, text_dir, pdf_file):
    # Create the PDF object, using the default page size
    c = canvas.Canvas(pdf_file, pagesize=pagesizes.letter)
    c.setFont("Helvetica", 12) 
    # Get a list of all the image and text files in the specified directories
    image_files = [f for f in os.listdir(image_dir) if f.endswith('.jpg') or f.endswith('.png')]
    text_files = [f for f in os.listdir(text_dir) if f.endswith('.txt')]
    y = 760
    #Iterate through the text files and add the content to the PDF
    for i, text_file in enumerate(text_files):
        with open(os.path.join(text_dir, text_file), 'r') as f:
            text = f.read()
            text_lines = text.split("\n")
            for line in text_lines:
                c.drawString(0, y, line)
                y -= 12
                if y < 50:
                    y = 760
    for i, image_file in enumerate(image_files):
        c.showPage() # Add new page before adding the image
        img = Image.open(os.path.join(image_dir, image_file))
        img_width, img_height = img.size
        aspect = img_height / float(img_width)
        c.drawImage(os.path.join(image_dir, image_file), 0, 200, width=612, height=aspect * 612)
    c.save()


# Example usage
create_pdf(args.image_dir, args.text_dir, args.pdf_file)
