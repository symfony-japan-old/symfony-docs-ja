post_max_size_message
~~~~~~~~~~~~~~~~~~~~~

**データ型**: ``string`` **デフォルト**: ``The uploaded file was too large. Please try to upload a smaller file.``

これは送信されたPOSTデータが ``php.ini`` の ``post_max_size`` ディレクティブを超えた場合に使われる検証エラーメッセージです。
``{{ max }}`` プレースホルダは、許可されたサイズを表示するために使用することができます。

.. note:

    ``post_max_size`` の検証はルートフォームのみ発生します。

.. 2014/06/20 yositani2002 d066f4d9e7518462c837d846a59bc5b8fc1ffcd9
