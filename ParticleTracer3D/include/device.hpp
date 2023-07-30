

class device{
    static int get_default_device();
    static void set_default_device(int device);

    private:
        static const int default_device = 0;

};