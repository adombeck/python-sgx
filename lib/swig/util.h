static void reverse_byte_array(uint8_t *array, size_t size)
{
    size_t i = 0;
    for(i = 0; i < size / 2; i++)
    {
        uint8_t temp = array[i];
        array[i] = array[size - i - 1];
        array[size - i - 1] = temp;
    }
}
