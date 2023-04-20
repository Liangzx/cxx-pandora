// std::string EncodeToString(const unsigned char *in, int inl)
// {
//     std::string out_str{};
//     auto block_size = inl * 4 / 3 + 3;
//     auto out = (char *)malloc(block_size);
//     if (0 != EVP_EncodeBlock(reinterpret_cast<unsigned char *>(out), in, inl))
//     {
//         out_str = out;
//     }
//     if (nullptr != out)
//     {
//         free(out);
//     }
//     return out_str;
// }

// std::string EncodeToString(const std::string &in)
// {
//     if (in.empty())
//     {
//         return std::string{};
//     }
//     std::string out_str{};
//     auto inl = (int)in.length();
//     auto block_size = inl * 4 / 3 + 3;
//     auto out = (char *)malloc(inl * block_size);
//     if (0 != EVP_EncodeBlock(reinterpret_cast<unsigned char *>(out), reinterpret_cast<const unsigned char *>(in.c_str()), inl))
//     {
//         out_str = out;
//     }
//     if (nullptr != out)
//     {
//         free(out);
//     }
//     return out_str;
// }