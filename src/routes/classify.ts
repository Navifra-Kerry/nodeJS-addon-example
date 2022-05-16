import * as express from 'express';
import { Request, Response } from 'express';
const router = express.Router();
import { Image, classify } from '../api';
import * as multer from 'multer';
const upload = multer();

router.post('/classify', upload.single('image'), async (req: Request, res: Response) => {
    const img: string = req.body.image;
    if (img) {
        const mat = Image.fromBase64(img);
        
        const reslut = classify(mat);
        return res.status(200).send(reslut);
    }

    return res.status(401).send()
});

export default router;
