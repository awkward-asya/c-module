from PIL import Image
import seam_carving


def index_in_array(i: int, j: int, width) -> int:
    return (j * width + i) * 3


def download_image(image_path: str) -> tuple:
    image = Image.open(image_path)
    width, height = image.size
    size = height * width * 3
    array = [0] * size
    for j in range(height):
        for i in range(width):
            r, g, b = image.getpixel((i, j))
            index = index_in_array(i, j, width)
            array[index] = r
            array[index + 1] = g
            array[index + 2] = b

    return array, width, height


def upload_image(res, final_width, final_height):
    data = bytes(res)
    image = Image.frombytes('RGB', (final_width, final_height), data)
    output_file = "result_image.jpg"
    try:
        image.save(output_file, "JPEG")
    except IOError as e:
        raise RuntimeError(str(e))


def resize_image(image_path: str, desired_width: int, desired_height: int):
    array, width, height = download_image(image_path)
    res = seam_carving.resize_as_variable(array, width, height, desired_width, desired_height)
    upload_image(res, width-desired_width,  height-desired_height)
