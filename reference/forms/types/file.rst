.. index::
   single: Forms; Fields; file

.. note::

    * 対象バージョン：2.3 (2.1以降)
    * 翻訳更新日：2013/11/23

file フィールドタイプ
===============

``file`` タイプはフォームにファイルを挿入する機能を果たします.

+-------------+---------------------------------------------------------------------+
| 対応するタグ| ``input`` ``file`` フィールド                                       |
+-------------+---------------------------------------------------------------------+
| 継承された  | - `required`_                                                       |
| オプション  | - `label`_                                                          |
|             | - `read_only`_                                                      |
|             | - `disabled`_                                                       |
|             | - `error_bubbling`_                                                 |
|             | - `error_mapping`_                                                  |
|             | - `mapped`_                                                         |
+-------------+---------------------------------------------------------------------+
| 親タイプ    | :doc:`form </reference/forms/types/form>`                           |
+-------------+---------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\FileType`  |
+-------------+---------------------------------------------------------------------+

基本的な使い方
-----------

このような定義のフォームがあったとして、:

.. code-block:: php

    $builder->add('attachment', 'file');

フォームが送信されると, ``attachment`` フィールドが
:class:`Symfony\\Component\\HttpFoundation\\File\\UploadedFile` のインスタンスになります. これは 添付ファイルを一時的な場所から保存先に移動させるために使用することができます。:

.. code-block:: php

    use Symfony\Component\HttpFoundation\File\UploadedFile;

    public function uploadAction()
    {
        // ...

        if ($form->isValid()) {
            $someNewFilename = ...

            $form['attachment']->getData()->move($dir, $someNewFilename);

            // ...
        }

        // ...
    }

``move()`` メソッドは引数としてディレクトリ名とファイル名を受け取ります。
以下のいずれかの方法でファイル名を計算します::

    // 元のままのファイル名を使う
    $file->move($dir, $file->getClientOriginalName());

    // ランダムなファイル名にし、拡張子を推測する (より安全な方法)
    $extension = $file->guessExtension();
    if (!$extension) {
        // 拡張子が推測できなかった場合
        $extension = 'bin';
    }
    $file->move($dir, rand(1, 99999).'.'.$extension);

元のままのファイル名と ``getClientOriginalName()`` を使う方法はエンドユーザーから操作されてしまう可能性があるため安全ではありません。
さらに、 許可されない文字列がファイル名に含まれてしまう可能性もあるので、事前に名前をサニタイズすることを推奨します。

:doc:`cookbook </cookbook/doctrine/file_uploads>` にファイルアップロードとDoctrineのentityとの紐付けを管理する方法の例が掲載されています。

継承されたオプション
-----------------

これらのオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています。:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. 2013/11/23 sotarof b675661199d466be4b6cb6f70d16aa1e3574c789
